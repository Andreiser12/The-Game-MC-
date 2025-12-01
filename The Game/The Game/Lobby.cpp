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



