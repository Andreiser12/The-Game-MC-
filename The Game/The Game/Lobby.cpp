module Lobby;

Lobby::Lobby(const std::string& lobbyID) : m_lobbyID(lobbyID) {}

bool Lobby::AddPlayer(unsigned playerId, std::shared_ptr<Player> player)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (IsFull()) return false;

	if (m_players.find(playerId) != m_players.end())
		return false;

	m_players[playerId] = player;

	if (m_players.size() == 1) m_hostId = playerId;


	if (m_players.size() >= K_MIN_PLAYERS)
		m_gameStarted.store(true);
	return true;
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
		m_hostId = 0;
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

