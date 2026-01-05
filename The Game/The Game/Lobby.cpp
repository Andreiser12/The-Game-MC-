module Lobby;

Lobby::Lobby(const std::string& lobbyId, unsigned hostId, const std::string& username) : 
	m_lobbyId{lobbyId},
	m_hostId{hostId}
{
	auto hostPlayer = std::make_shared<Player>(hostId, username);
	m_players.emplace(hostId, hostPlayer);
}

bool Lobby::AddPlayer(unsigned playerId, const std::string& username)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (IsFull()) return false;

	if (m_players.find(playerId) != m_players.end()) return false;

	auto player = std::make_shared<Player>(playerId, username);
	m_players[playerId] = player;
	m_lastPingPlayer[playerId] = std::chrono::steady_clock::now();

	if (m_players.size() == 1) m_hostId = playerId;
	return true;
}

void Lobby::AddChatMessage(const std::string& username, const std::string& message)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_chatBuffer.push_back({
		++m_lastMessageId,
		username,
		message
		});

	if (m_chatBuffer.size() > kMaxChatMessages)
	{
		m_chatBuffer.pop_front();
	}
}

std::optional<std::vector<ChatMessage>> Lobby::GetMessagesAfter(unsigned afterId) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::vector<ChatMessage> out;
	out.reserve(kMaxChatMessages);
	for (const auto& message : m_chatBuffer)
	{
		if (message.id > afterId) out.push_back(message);
	}
	if (out.empty()) return std::nullopt;
	return std::make_optional(out);
}

RemoveResult Lobby::RemovePlayer(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto it = m_players.find(playerId);
	if (it == m_players.end()) return RemoveResult::NotFound;

	bool isHost = (m_hostId == playerId);

	m_players.erase(it);
	m_lastPingPlayer.erase(playerId);

	if (isHost) 
	{
		m_hostId = 0u;
		return RemoveResult::HostRemoved;
	}
	if (m_players.size() < kMinPlayers)
	{
		m_gameStarted.store(false);
	}
	return RemoveResult::Removed;
}

RemoveResult Lobby::RemoveMatchPlayer(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	if (!m_gameStarted || !m_table) return RemoveResult::NotFound;

	bool isHost = (m_hostId == playerId);
	
	m_players.erase(playerId);
	m_lastPingPlayer.erase(playerId);

	return RemoveResult::Removed;
}

void Lobby::UpdatePlayerPing(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto iterator = m_lastPingPlayer.find(playerId);
	if (iterator != m_lastPingPlayer.end())
	{
		iterator->second = std::chrono::steady_clock::now();
	}
}

std::vector<unsigned> Lobby::GetInactivePlayers(std::chrono::seconds timeout)
{
	std::vector<unsigned> inactive;
	auto now = std::chrono::steady_clock::now();

	std::lock_guard<std::mutex> lock(m_mutex);
	for (auto& [playerId, lastPing] : m_lastPingPlayer)
	{
		if (now - lastPing > timeout)
			inactive.push_back(playerId);
	}
	return inactive;
}

bool Lobby::IsFull() const
{
	return m_players.size() == kMaxPlayers;
}

bool Lobby::StartGame(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (m_gameStarted.load()) return false;

	if (playerId != m_hostId) return false;

	if (m_players.size() < kMinPlayers) return false;

	std::vector<std::pair<unsigned, std::string>> lobbyPlayers;
	for (const auto& [id, player] : m_players)
	{
		lobbyPlayers.push_back({ id, player->GetUsername() });
	}

	m_table = std::make_unique<Table>(lobbyPlayers);
	m_gameStarted.store(true);
	return true;
}

bool Lobby::HasGameStarted() const
{
	return m_gameStarted.load();
}

const std::unordered_map<unsigned, std::shared_ptr<Player>>& Lobby::GetPlayers() const
{
	return m_players;
}

bool Lobby::IsPlayerHost(unsigned playerId) const
{
	return m_hostId == playerId;
}

unsigned Lobby::GetHostId() const
{
	return m_hostId;
}

std::optional<std::vector<uint8_t>> Lobby::GetPlayerCards(unsigned playerId) const
{
	std::lock_guard lock(m_mutex);

	if (!m_gameStarted || !m_table) return std::nullopt;

	const auto& player = m_table->GetPlayer(playerId);
	std::vector<uint8_t> playerCards;
	for (const auto& card : player.GetCards())
	{
		playerCards.emplace_back(card.GetId());
	}
	std::sort(playerCards.begin(), playerCards.end());
	return playerCards;
}

bool Lobby::PlayCard(unsigned playerId, uint8_t cardId, uint8_t deckId)
{
	std::lock_guard lock(m_mutex);
	if (!m_gameStarted || !m_table) return false;

	auto& player = m_table->GetPlayer(playerId);
	Card<uint8_t> card(cardId);
	if (!m_table->PlayCard(playerId, deckId, card)) return false;

	m_table->PlaceCardOnDeck(deckId, card);
	player.RemoveCard(cardId);
	player.IncrementCardsPlaced();
	return true;
}

std::optional<std::vector<DeckState>> Lobby::GetDecksState() const
{
	std::lock_guard lock(m_mutex);

	if (!m_gameStarted || !m_table) return std::nullopt;

	std::vector<DeckState> decksState;
	for (size_t i = 0; i < 4; i++)
	{
		DeckState deck;
		deck.deckId = static_cast<uint8_t>(i);
		deck.topCardId = m_table->GetDeck(static_cast<uint8_t>(i)).GetTopCard().GetId();
		decksState.emplace_back(deck);
	}
	return decksState;
}

std::optional<size_t> Lobby::GetDrawPileCount() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (!m_gameStarted || !m_table) return std::nullopt;

	return m_table->GetDrawPileCount();
}

std::optional<EndTurnState> Lobby::GetEndTurnState(unsigned playerId) const
{
	std::lock_guard lock(m_mutex);
	if (!m_gameStarted || !m_table) return std::nullopt;

	auto& player = m_table->GetPlayer(playerId);
	EndTurnState state;
	state.cardsPlaced = player.GetCardsPlacedRound();
	const bool pileEmpty = m_table->GetDrawPileCount() == 0;
	state.minRequiredCards = (pileEmpty == true) ? 1u : 2u;
	state.canEndTurn = state.cardsPlaced >= state.minRequiredCards;
	return state;
}

std::optional<TurnState> Lobby::GetTurnState(unsigned playerId) const
{
	std::lock_guard lock(m_mutex);
	if (!m_gameStarted || !m_table) return std::nullopt;

	TurnState state;
	state.currentPlayerId = m_table->GetCurrentPlayerId();
	state.isMyTurn = (state.currentPlayerId == playerId);
	return state;
}

std::optional<std::vector<uint8_t>> Lobby::EndTurn(unsigned playerId)
{
	if (!m_gameStarted || !m_table) return std::nullopt;
	m_table->EndTurn(playerId);
	return GetPlayerCards(playerId);
}

std::optional<GameResult> Lobby::GetGameResult() const
{
	if (!m_gameStarted || !m_table) return std::nullopt;
	return m_table->GetGameResult();
}

std::optional<std::array<bool, 4>> Lobby::GetValidDecks(uint8_t cardId) const
{
	if (!m_gameStarted || !m_table) return std::nullopt;
	
	std::array<bool, 4> validDecks;
	for (uint8_t deckId = 0u; deckId < 4u; deckId++)
	{
		auto deck = m_table->GetDeck(deckId);
		switch (deck.GetDeckType())
		{
		case Deck::DeckType::Ascending:
			validDecks[deckId] = cardId > deck.GetTopCard().GetId();
			break;
		case Deck::DeckType::Descending:
			validDecks[deckId] = cardId < deck.GetTopCard().GetId();
			break;
		}
	}
	return std::make_optional(validDecks);
}
