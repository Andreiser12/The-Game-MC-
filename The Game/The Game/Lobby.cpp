module Lobby;

Lobby::Lobby(uint32_t lobbyID, uint32_t minPlayers, uint32_t maxPlayers)
	:	m_lobbyID(lobbyID), m_minPlayersToStart(minPlayers), 
		m_maxPlayers(maxPlayers), m_gameStarted(false)
{

}

bool Lobby::AddPlayer(std::shared_ptr<Player> player)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (!player || IsFull()) return false;
	m_players.push_back(player);
	if (m_players.size() >= m_minPlayersToStart)
		m_gameStarted.store(true);
	return true;
}

bool Lobby::RemovePlayer(std::shared_ptr<Player> player)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (!player) return false;
	auto iterator = std::find(m_players.begin(), m_players.end(), player);
	if (iterator != m_players.end())
	{
		m_players.erase(iterator);
		return true;
	}
	return false;
}

bool Lobby::IsFull() const
{
	return m_players.size() >= m_maxPlayers;
}

bool Lobby::HasGameStarted() const
{
	return m_gameStarted.load();
}

const std::vector<std::shared_ptr<Player>>& Lobby::GetPlayers() const
{
	return m_players;
}

