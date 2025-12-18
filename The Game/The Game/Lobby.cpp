module Lobby;

Lobby::Lobby(const std::string& lobbyId, unsigned hostId, const std::string& username) : 
	m_lobbyId{lobbyId},
	m_hostId{hostId}
{
	auto hostPlayer = std::make_shared<Player>(hostId, username);
	m_players.emplace(hostId, hostPlayer);
}

bool Lobby::AddPlayer(unsigned playerId, const std::string& playerUsername)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (IsFull()) return false;

	if (m_players.find(playerId) != m_players.end())
		return false;

	auto player = std::make_shared<Player>(playerId, playerUsername);
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

std::optional<std::vector<unsigned>> Lobby::GetPlayerCards(unsigned playerId) const
{
	std::lock_guard lock(m_mutex);

	if (!m_gameStarted || !m_table) return std::nullopt;

	const auto& players = m_table->GetPlayersMap();
	auto it = players.find(playerId);
	if (it == players.end()) return std::nullopt;

	std::vector<unsigned> playerCards;
	for (const auto& card : it->second.GetCards())
	{
		playerCards.push_back(card.getId());
	}
	return playerCards;
}

bool Lobby::PlayCard(unsigned playerId, unsigned cardId, unsigned deckIndex)
{
	std::lock_guard lock(m_mutex);

	if (!m_gameStarted || !m_table) return false;

	return true;
}
