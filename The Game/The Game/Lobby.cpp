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

	if (m_players.size() == 1) m_hostId = playerId;


	if (m_players.size() >= K_MIN_PLAYERS)
		m_gameStarted.store(true);
	return true;
}

void Lobby::AddChatMessage(const std::string& username, const std::string& message)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_chatMessages.push_back({ ++m_lastMessageId, username, message });
}

RemoveResult Lobby::RemovePlayer(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto it = m_players.find(playerId);
	if (it == m_players.end()) return RemoveResult::NotFound;

	bool isHost = (m_hostId == playerId);

	m_players.erase(it);

	if (isHost) 
	{
		m_hostId = 0u;
		return RemoveResult::HostRemoved;
	}
	if (m_players.size() < K_MIN_PLAYERS)
	{
		m_gameStarted.store(false);
	}
	return RemoveResult::Removed;
}

bool Lobby::IsFull() const
{
	return m_players.size() == K_MAX_PLAYERS;
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

const std::vector<ChatMessage>& Lobby::GetChatMessages() const
{
	return m_chatMessages;
}

