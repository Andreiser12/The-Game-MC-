export module Lobby;

import <stdint.h>;
import <vector>;
import <deque>;
import <unordered_map>;
import <atomic>;
import <mutex>;
import <chrono>;

import Player;
import Table;
import Card;

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
	explicit Lobby(const std::string& lobbyId, unsigned hostId, const std::string& username);
	bool AddPlayer(unsigned playerId, const std::string& playerUsername);
	void AddChatMessage(const std::string& username, const std::string& message);
	std::optional<std::vector<ChatMessage>> GetMessagesAfter(unsigned afterId) const;

	RemoveResult RemovePlayer(unsigned playerId);
	void UpdatePlayerPing(unsigned playerId);
	std::vector<unsigned> GetInactivePlayers(std::chrono::seconds timeout);

	bool IsFull() const;
	bool StartGame(unsigned playerId);
	bool HasGameStarted() const;
	const std::unordered_map<unsigned, std::shared_ptr<Player>>& GetPlayers() const;
	bool IsPlayerHost(unsigned playerId) const;
	unsigned GetHostId() const;

	std::optional<std::vector<unsigned>> GetPlayerCards(unsigned playerId) const;
	bool PlayCard(unsigned playerId, unsigned cardId, unsigned deckIndex);

private:
	Lobby() = delete;

public:
	inline static constexpr size_t kMaxChatMessages{ 200 };
	inline static constexpr uint8_t kMinPlayers{ 2u };
	inline static constexpr uint8_t kMaxPlayers{ 5u };

private:
	std::string m_lobbyId;
	unsigned m_hostId;

	std::unordered_map<unsigned, std::shared_ptr<Player>> m_players;
	std::unordered_map<unsigned, std::chrono::steady_clock::time_point> m_lastPingPlayer;
	unsigned m_lastMessageId{ 0u };
	std::deque<ChatMessage> m_chatBuffer;
	std::atomic<bool> m_gameStarted{ false };
	std::unique_ptr<Table> m_table;
	mutable std::mutex m_mutex;

};