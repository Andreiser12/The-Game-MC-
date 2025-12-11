export module Lobby;

import <stdint.h>;
import <vector>;
import <unordered_map>;
import <atomic>;
import <mutex>;
import <chrono>;

import Player;

export enum class RemoveResult
{
	NotFound,
	Removed,
	HostRemoved
};

export struct ChatMessage
{
	unsigned id;
	std::string username;
	std::string message;
};

export class Lobby
{
public:
	Lobby() = delete;
	explicit Lobby(const std::string& lobbyId, unsigned hostId, const std::string& username);
	bool AddPlayer(unsigned playerId, const std::string& playerUsername);
	void AddChatMessage(const std::string& username, const std::string& message);

	RemoveResult RemovePlayer(unsigned playerId);
	void UpdatePlayerPing(unsigned playerId);
	std::vector<unsigned> GetInactivePlayers(std::chrono::seconds timeout);

	bool IsFull() const;
	bool HasGameStarted() const;
	const std::unordered_map<unsigned, std::shared_ptr<Player>>& GetPlayers() const;
	bool IsPlayerHost(unsigned playerId) const;
	unsigned GetHostId() const;

	const std::vector<ChatMessage>& GetChatMessages() const;

public:
	static constexpr uint8_t K_MIN_PLAYERS{ 2u };
	static constexpr uint8_t K_MAX_PLAYERS{ 5u };

private:
	std::string m_lobbyId;
	unsigned m_hostId;	

	std::unordered_map<unsigned, std::shared_ptr<Player>> m_players;
	std::unordered_map<unsigned, std::chrono::steady_clock::time_point> m_lastPingPlayer;

	std::vector<ChatMessage> m_chatMessages;
	unsigned m_lastMessageId{ 0 };
	std::atomic<bool> m_gameStarted{ false };
	std::mutex m_mutex;
};