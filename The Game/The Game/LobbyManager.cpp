module LobbyManager;

LobbyManager& LobbyManager::Instance()
{
	static LobbyManager instance;
	return instance;
}
std::shared_ptr<Lobby> LobbyManager::CreateLobby(const std::string& lobbyId, unsigned hostId,
	const std::string& hostUsername)
{
	std::lock_guard lock(m_mutex);

	if (m_lobbies.contains(lobbyId)) return nullptr;

	auto lobby = std::make_shared<Lobby>(lobbyId, hostId, hostUsername);
	m_lobbies.emplace(lobbyId, lobby);
	return lobby;
}

std::shared_ptr<Lobby> LobbyManager::GetLobby(const std::string& id)
{
	std::lock_guard lock(m_mutex);

	if (auto it = m_lobbies.find(id);  it != m_lobbies.end())
	{
		return it->second;
	}
	return nullptr;
}

bool LobbyManager::RemoveLobby(const std::string& id)
{
	std::lock_guard lock(m_mutex);
	auto it = m_lobbies.find(id);
	if (it != m_lobbies.end())
	{
		m_lobbies.erase(id);
		return true;
	}
	return false;
}