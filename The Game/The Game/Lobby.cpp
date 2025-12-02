module Lobby;

Lobby::Lobby(uint32_t lobbyID, uint32_t minPlayers, uint32_t maxPlayers)
	:	m_lobbyID(lobbyID), m_minPlayersToStart(minPlayers), 
		m_maxPlayers(maxPlayers), m_gameStarted(false)
{

}

bool Lobby::AddPlayer(std::shared_ptr<Player> player)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (IsFull()) return false;

	unsigned playerID = player->GetPlayerID();
	if (m_players.find(playerID) != m_players.end())
		return false;

	m_players[playerID] = player;

	if (m_players.size() >= m_minPlayersToStart)
		m_gameStarted.store(true);
	return true;
}

bool Lobby::RemovePlayer(unsigned playerID)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto iterator = m_players.find(playerID);
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

const std::unordered_map<unsigned, std::shared_ptr<Player>>& Lobby::GetPlayers() const
{
	return m_players;
}

