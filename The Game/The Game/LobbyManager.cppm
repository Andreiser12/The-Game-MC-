export module LobbyManager;

import Lobby;
import <iostream>;
import <mutex>;
import <unordered_map>;

export class LobbyManager
{
public:
	static LobbyManager& Instance();
	std::shared_ptr<Lobby> CreateLobby(uint32_t id, uint32_t minPlayers, uint32_t maxPlayers);
	std::shared_ptr<Lobby> GetLobby(uint32_t id);
	bool RemoveLobby(uint32_t id);

private:
	std::unordered_map<uint32_t, std::shared_ptr<Lobby>> m_lobbies;
	std::mutex m_mutex;

private:
	LobbyManager() = default;
	~LobbyManager() = default;

	LobbyManager(const LobbyManager&) = delete;
	LobbyManager& operator= (const LobbyManager&) = delete;

	LobbyManager(LobbyManager&&) = delete;
	LobbyManager& operator=(LobbyManager&&) = delete;

};