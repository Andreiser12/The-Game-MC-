export module Lobby;

import <stdint.h>;
import <vector>;
import <atomic>;
import <mutex>;
import Player;

export class Lobby 
{
	private:
		uint32_t m_lobbyID;
		uint32_t m_maxPlayers;
		std::vector<std::shared_ptr<Player>> m_players;
		std::atomic<bool> m_gameStarted;
		std::mutex m_mutex;
	public:
		Lobby() = delete;
		explicit Lobby(uint32_t lobbyID, uint32_t maxPlayers);
		bool AddPlayer(std::shared_ptr<Player> player);
		bool RemovePlayer(std::shared_ptr<Player> player);
		bool IsFull() const;
		bool HasGameStarted() const;
		const std::vector<std::shared_ptr<Player>>& GetPlayers() const;
};