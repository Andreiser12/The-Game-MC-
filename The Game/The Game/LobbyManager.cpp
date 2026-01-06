module LobbyManager;

LobbyManager& LobbyManager::Instance()
{
	static LobbyManager instance;
	
	static bool initialized = false;
	if (!initialized)
	{
		initialized = true;
		std::thread([]() {
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::seconds(5));
				auto& manager = LobbyManager::Instance();
				//manager.CleanupInactivePlayers();
			}
			}).detach();
	}

	return instance;

}
std::shared_ptr<Lobby> LobbyManager::CreateLobby(const std::string& lobbyId, unsigned hostId,
	const std::string& hostUsername)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_lobbies.contains(lobbyId))
		return nullptr;

	auto lobby = std::make_shared<Lobby>(lobbyId, hostId, hostUsername);
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

//void LobbyManager::CleanupInactivePlayers()
//{
//	std::lock_guard<std::mutex> lock(m_mutex);
//
//	for (auto& [lobbyId, lobby] : m_lobbies)
//	{	
//		auto inactivePlayers = lobby->GetInactivePlayers(std::chrono::seconds(15));
//		for (auto playerId : inactivePlayers)
//		{
//			auto result = lobby->RemovePlayer(playerId);
//			if (result == RemoveResult::HostRemoved)
//			{
//				m_lobbies.erase(lobbyId);
//				break;
//			}
//		}
//	}
//}

