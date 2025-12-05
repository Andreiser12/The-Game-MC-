module LobbyManager;

LobbyManager& LobbyManager::Instance()
{
	static LobbyManager instance;
	return instance;
}
std::shared_ptr<Lobby> LobbyManager::CreateLobby(const std::string& lobbyId)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_lobbies.contains(lobbyId))
		return nullptr;

	auto lobby = std::make_shared<Lobby>(lobbyId);
	m_lobbies[lobbyId] = lobby;
	return lobby;
}

std::shared_ptr<Lobby> LobbyManager::GetLobby(const std::string& id)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto iterator = m_lobbies.find(id);
	if (iterator != m_lobbies.end())
		return iterator->second;
	return nullptr;
}

bool LobbyManager::RemoveLobby(const std::string& id)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	auto iterator = m_lobbies.find(id);
	if (iterator != m_lobbies.end())
	{
		m_lobbies.erase(iterator);
		return true;
	}
	return false;
}

