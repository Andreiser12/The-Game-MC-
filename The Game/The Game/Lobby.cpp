module Lobby;

Lobby::Lobby(std::string_view lobbyId, unsigned hostId, const std::string& username) :
	m_lobbyId{ lobbyId },
	m_hostId{ hostId }
{
	m_players.try_emplace(hostId, hostId, username);
	m_lastPingPlayer[hostId] = std::chrono::steady_clock::now();
}

bool Lobby::AddPlayer(unsigned playerId, const std::string& username)
{
	std::lock_guard lock(m_mutex);
	if (IsFull() || m_players.contains(playerId)) return false;

	m_players.try_emplace(playerId, playerId, username);
	m_lastPingPlayer[playerId] = std::chrono::steady_clock::now();

	if (m_players.size() == 1) m_hostId = playerId;
	return true;
}

void Lobby::AddChatMessage(std::string_view username, std::string_view message, bool isSystem)
{
	std::lock_guard lock(m_mutex);

	std::string finalMessage;

	if (isSystem)
	{
		finalMessage = std::string(message);
		m_chatBuffer.emplace_back(ChatMessage{
		.id = ++m_lastMessageId,
		.username = "",
		.message = finalMessage,
		.isSystem = isSystem
			});
	}

	else
	{
		finalMessage = ChatFilter::FilterMessage(std::string(message));
		m_chatBuffer.emplace_back(ChatMessage{
		.id = ++m_lastMessageId,
		.username = std::string(username),
		.message = finalMessage,
		.isSystem = isSystem
		});
	}

	TrimChatBuffer();
}

std::optional<std::vector<ChatMessage>> Lobby::GetMessagesAfter(unsigned afterId) const
{
	std::lock_guard lock(m_mutex);
	std::vector<ChatMessage> result;
	result.reserve(kMaxChatMessages);

	std::ranges::copy_if(m_chatBuffer, std::back_inserter(result),
		[afterId](const auto& msg) { return msg.id > afterId; });

	return std::make_optional(result);
}

RemoveResult Lobby::RemovePlayer(unsigned playerId)
{
	std::lock_guard lock(m_mutex);

	auto it = m_players.find(playerId);
	if (it == m_players.end()) return RemoveResult::NotFound;

	const bool isHost{ (m_hostId == playerId) };

	m_players.erase(it);
	m_lastPingPlayer.erase(playerId);

	if (isHost)
	{
		m_hostId = 0u;
		return RemoveResult::HostRemoved;
	}
	if (m_players.size() < kMinPlayers)
	{
		m_gameStarted.store(false, std::memory_order_release);
	}
	return RemoveResult::Removed;
}

void Lobby::MarkPlayerAsDisconnected(unsigned playerId) noexcept
{
	std::lock_guard lock(m_mutex);

	auto it = m_players.find(playerId);
	if (it == m_players.end()) return;

	it->second.isConnected = false;

	if (m_table) m_table->RemovePlayer(playerId);
	m_lastPingPlayer.erase(playerId);
}

RemoveResult Lobby::RemoveMatchPlayer(unsigned playerId)
{
	std::lock_guard lock(m_mutex);
	if (!IsGameActive()) return RemoveResult::NotFound;

	if (auto playerIt = m_players.find(playerId); playerIt != m_players.end())
	{
		playerIt->second.isConnected = false;
	}

	if (!m_table->RemovePlayer(playerId)) return RemoveResult::NotFound;

	m_lastPingPlayer.erase(playerId);

	size_t activePlayers{ 0 };
	for (const auto& [id, player] : m_players)
	{
		if (player.isConnected && m_table->HasPlayer(id))
		{
			++activePlayers;
		}
	}
	if (activePlayers < kMinPlayers) m_gameResult = GameResult::NotEnoughPlayers;
	return RemoveResult::Removed;
}

void Lobby::UpdatePlayerPing(unsigned playerId) noexcept
{
	std::lock_guard lock(m_mutex);

	if (auto it = m_lastPingPlayer.find(playerId); it != m_lastPingPlayer.end())
	{
		it->second = std::chrono::steady_clock::now();
	}
}

std::vector<unsigned> Lobby::GetInactivePlayers(std::chrono::seconds timeout) const
{
	auto now = std::chrono::steady_clock::now();
	std::vector<unsigned> inactive;
	inactive.reserve(m_lastPingPlayer.size());

	std::lock_guard lock(m_mutex);
	for (const auto& [playerId, lastPing] : m_lastPingPlayer)
	{
		if (now - lastPing > timeout)
			inactive.emplace_back(playerId);
	}
	return inactive;
}

bool Lobby::IsFull() const noexcept
{
	return m_players.size() == kMaxPlayers;
}

size_t Lobby::GetPlayerCount() const noexcept
{
	return m_players.size();
}

bool Lobby::StartGame(unsigned playerId)
{
	std::lock_guard lock(m_mutex);

	if (m_gameStarted.load(std::memory_order_acquire)) return false;
	if (playerId != m_hostId) return false;
	if (m_players.size() < kMinPlayers) return false;

	std::vector<std::pair<unsigned, std::string_view>> lobbyPlayers;
	lobbyPlayers.reserve(m_players.size());
	for (const auto& [id, player] : m_players)
	{
		lobbyPlayers.emplace_back(id, player.username);
	}

	m_table = std::make_unique<Table>
		(std::span<std::pair<unsigned, std::string_view>>{lobbyPlayers});

	m_gameStartTime = std::chrono::steady_clock::now();
	m_gameStarted.store(true, std::memory_order_release);
	return true;
}

const std::unordered_map<unsigned, PlayerInfo>& Lobby::GetPlayers() const
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

void Lobby::SetGameLost()
{
	m_gameResult = GameResult::Lost;
}

void Lobby::TrimChatBuffer() noexcept
{
	if (m_chatBuffer.size() > kMaxChatMessages) m_chatBuffer.pop_front();
}

bool Lobby::IsGameActive() const noexcept
{
	return m_gameStarted.load(std::memory_order_acquire) && m_table != nullptr;
}

std::optional<std::vector<uint8_t>> Lobby::GetPlayerCards(unsigned playerId) const
{
	std::lock_guard lock(m_mutex);

	if (!IsGameActive()) return std::nullopt;

	const auto& player = m_table->GetPlayer(playerId);
	std::vector<uint8_t> cards;
	cards.reserve(player.GetCardsCount());

	std::ranges::transform(player.GetCards(), std::back_inserter(cards),
		[](const auto& card) { return card.GetId();  });

	std::ranges::sort(cards);
	return cards;
}

bool Lobby::PlayCard(unsigned playerId, uint8_t cardId, uint8_t deckId)
{
	std::lock_guard lock(m_mutex);
	if (!IsGameActive()) return false;

	auto& player = m_table->GetPlayer(playerId);
	const Card<uint8_t> card{ cardId };
	if (!m_table->CanPlaceCard(playerId, deckId, card)) return false;

	m_table->PlaceCardOnDeck(deckId, std::move(card));
	player.RemoveCard(cardId);
	player.IncrementCardsPlaced();
	return true;
}

std::optional<std::vector<DeckState>> Lobby::GetDecksState() const
{
	std::lock_guard lock(m_mutex);

	if (!IsGameActive()) return std::nullopt;

	std::vector<DeckState> decksState;
	for (uint8_t i = 0; i < kDeckCount; ++i)
	{
		decksState.emplace_back(DeckState{
			.deckId = i,
			.topCardId = m_table->GetDeck(i).GetTopCard().GetId(),
			.size = m_table->GetDeck(i).GetDeckSize()
			});
	}
	return decksState;
}

std::optional<size_t> Lobby::GetDrawPileCount() const
{
	std::lock_guard lock(m_mutex);

	if (!IsGameActive()) return std::nullopt;

	return m_table->GetDrawPileCount();
}

std::optional<EndTurnState> Lobby::GetEndTurnState(unsigned playerId) const
{
	std::lock_guard lock(m_mutex);

	if (!IsGameActive()) return std::nullopt;

	const auto& player = m_table->GetPlayer(playerId);
	const uint8_t cardsPlaced{ player.GetCardsPlacedRound() };
	const bool pileEmpty{ m_table->GetDrawPileCount() == 0 };
	const uint8_t minRequired{ pileEmpty ? 1u : 2u };
	return EndTurnState{
		.playerId = playerId,
		.cardsPlaced = cardsPlaced,
		.minRequiredCards = minRequired,
		.canEndTurn = cardsPlaced >= minRequired
	};
}

std::optional<TurnState> Lobby::GetTurnState(unsigned playerId) const
{
	std::lock_guard lock(m_mutex);
	if (!IsGameActive()) return std::nullopt;

	const unsigned currentPlayerId{ m_table->GetCurrentPlayerId() };

	std::string currentName = "";
	if (m_players.contains(currentPlayerId))
	{
		currentName = m_players.at(currentPlayerId).username;
	}

	return TurnState{
		.currentPlayerId = currentPlayerId,
		.isMyTurn = currentPlayerId == playerId,
		.currentUsername = currentName
	};
}

std::optional<std::vector<uint8_t>> Lobby::EndTurn(unsigned playerId)
{
	if (!IsGameActive()) return std::nullopt;

	m_table->EndTurn(playerId);
	UpdateGameResult();
	return GetPlayerCards(playerId);
}

void Lobby::UpdateGameResult() noexcept
{
	if (!IsGameActive()) return;

	if (m_gameResult == GameResult::Lost) return;

	if (m_table->CheckWinCondition())
	{
		m_gameResult = GameResult::Won;
		return;
	}
	if (m_table->CheckLoseCondition()) m_gameResult = GameResult::Lost;
}

std::optional<GameResult> Lobby::GetGameResult() const
{
	if (!IsGameActive()) return std::nullopt;
	return m_gameResult;
}

double Lobby::GetMatchDurationHours() const
{
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<double> seconds = now - m_gameStartTime;
	return seconds.count() / kSecondsPerHour;
}

bool Lobby::StatsAlreadyUpdated() const noexcept
{
	return m_statsUpdated;
}

void Lobby::MarkStatsUpdated() noexcept
{
	m_statsUpdated = true;
}

std::optional<std::array<bool, 4>> Lobby::GetValidDecks(uint8_t cardId) const
{
	std::lock_guard lock(m_mutex);

	if (!IsGameActive()) return std::nullopt;

	std::array<bool, 4> validDecks;
	for (uint8_t deckId = 0u; deckId < kDeckCount; ++deckId)
	{
		const auto& deck = m_table->GetDeck(deckId);
		const uint8_t topCard{ deck.GetTopCard().GetId() };

		validDecks[deckId] = (deck.GetDeckType() == Deck::DeckType::Ascending)
			? (cardId > topCard)
			: (cardId < topCard);
	}
	return std::make_optional(validDecks);
}

std::optional<std::vector<PlayerState>> Lobby::GetPlayerStates() const
{
	std::lock_guard lock(m_mutex);

	if (!IsGameActive()) return std::nullopt;

	std::vector<PlayerState> states;
	states.reserve(m_players.size());
	for (const auto& [playerId, playerInfo] : m_players)
	{
		states.emplace_back(
			PlayerState{
				.playerId = playerId,
				.username = playerInfo.username,
				.isInMatch = playerInfo.isConnected && m_table->HasPlayer(playerId)
			});
	}
	return states;
}

std::optional<bool> Lobby::GetCanPlayMore(unsigned playerId, uint8_t remainingCards) const
{
	if (!m_gameStarted || !m_table) return std::nullopt;

	const auto& player = m_table->GetPlayer(playerId);
	return m_table->CanPlayerPlayMinCards(player, remainingCards);
}

std::optional<std::vector<std::pair<uint8_t, uint8_t>>> Lobby::GetAICards(unsigned playerId,
	uint8_t cardsRequired)
{
	if (!m_gameStarted || !m_table) return std::nullopt;

	return m_table->RunBotTurn(playerId, cardsRequired);
}
