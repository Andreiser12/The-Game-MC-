export module LobbyManager;

import Lobby;
import <mutex>;
import <unordered_map>;

export class LobbyManager
{
public:
	static LobbyManager& Instance();

	std::shared_ptr<Lobby> CreateLobby(const std::string& lobbyId, unsigned hostId,
		const std::string& hostUsername);
	std::shared_ptr<Lobby> GetLobby(const std::string& id);
	bool RemoveLobby(const std::string& id);

private:
	LobbyManager() = default;
	~LobbyManager() = default;

	LobbyManager(const LobbyManager&) = delete;
	LobbyManager& operator= (const LobbyManager&) = delete;
	LobbyManager(LobbyManager&&) = delete;
	LobbyManager& operator=(LobbyManager&&) = delete;

private:
	std::unordered_map<std::string, std::shared_ptr<Lobby>> m_lobbies;
	std::mutex m_mutex;
};