module LobbyManager;

LobbyManager& LobbyManager::Instance()
{
	static LobbyManager instance;
	return instance;
}
std::shared_ptr<Lobby> LobbyManager::CreateLobby(uint32_t id, uint32_t minPlayers, uint32_t maxPlayers)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_lobbies.contains(id))
		return nullptr;

	auto lobby = std::make_shared<Lobby>(id, minPlayers, maxPlayers);
	m_lobbies[id] = lobby;
	return lobby;
}