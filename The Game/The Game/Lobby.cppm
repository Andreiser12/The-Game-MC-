export module Lobby;

import <stdint.h>;
import <vector>;
import <unordered_map>;
import <atomic>;
import <mutex>;

import Player;

export class Lobby
{
public:
	Lobby() = delete;
	explicit Lobby(uint32_t lobbyID, uint32_t minPlayers, uint32_t maxPlayers);
	bool AddPlayer(std::shared_ptr<Player> player);
	bool RemovePlayer(unsigned playerID);
	bool IsFull() const;
	bool HasGameStarted() const;
	const std::unordered_map<unsigned, std::shared_ptr<Player>>& GetPlayers() const;

private:
	uint32_t m_lobbyID;
	uint32_t m_minPlayersToStart;
	uint32_t m_maxPlayers;
	std::unordered_map<unsigned, std::shared_ptr<Player>> m_players;
	std::atomic<bool> m_gameStarted;
	std::mutex m_mutex;
};