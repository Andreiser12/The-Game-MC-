export module Lobby;

import <stdint.h>;
import <vector>;
import <array>;
import <unordered_map>;
import <deque>;
import <atomic>;
import <mutex>;
import <chrono>;
import <algorithm>;

import Player;
import Table;
import Card;
import Deck;

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

export struct DeckState
{
	uint8_t deckId;
	uint8_t topCardId;
};

export struct EndTurnState
{
	bool canEndTurn;
	uint8_t cardsPlaced;
	uint8_t minRequiredCards;
};

export struct TurnState
{
	bool isMyTurn;
	unsigned currentPlayerId;
};

export enum class GameResult
{
	Ongoing,
	Won,
	Lost,
	NotEnoughPlayers
};

export struct PlayerState
{
	std::string username;
	bool isInMatch;
};

export struct PlayerInfo
{
	unsigned playerId;
	std::string username;
	bool isConnected{ true };

	PlayerInfo (unsigned p, std::string u) : playerId{p}, username{std::move(u)} {}
};

export class Lobby
{
public:
	explicit Lobby(const std::string& lobbyId, unsigned hostId, const std::string& username);
	bool AddPlayer(unsigned playerId, const std::string& playerUsername);
	void AddChatMessage(const std::string& username, const std::string& message);
	std::optional<std::vector<ChatMessage>> GetMessagesAfter(unsigned afterId) const;
	std::optional<std::vector<DeckState>> GetDecksState() const;
	std::optional<size_t> GetDrawPileCount() const;
	std::optional<EndTurnState> GetEndTurnState(unsigned playerId) const;
	std::optional<TurnState> GetTurnState(unsigned playerId) const;
	std::optional<GameResult> GetGameResult() const;
	std::optional<std::vector<uint8_t>> GetPlayerCards(unsigned playerId) const;
	std::optional<std::array<bool, 4>> GetValidDecks(uint8_t cardId) const;
	std::optional<std::vector<PlayerState>> GetPlayerStates() const;

	RemoveResult RemovePlayer(unsigned playerId);
	void MarkPlayerAsDisconnected(unsigned playerId);
	RemoveResult RemoveMatchPlayer(unsigned playerId);
	void UpdatePlayerPing(unsigned playerId);
	std::vector<unsigned> GetInactivePlayers(std::chrono::seconds timeout);

	bool IsFull() const;
	bool StartGame(unsigned playerId);
	bool HasGameStarted() const;
	const std::unordered_map<unsigned, PlayerInfo>& GetPlayers() const;
	bool IsPlayerHost(unsigned playerId) const;
	unsigned GetHostId() const;

	bool PlayCard(unsigned playerId, uint8_t cardId, uint8_t deckId);
	std::optional<std::vector<uint8_t>> EndTurn(unsigned playerId);
	void UpdateGameResult();
 
private:
	Lobby() = delete;

public:
	inline static constexpr size_t kMaxChatMessages{ 100 };
	inline static constexpr uint8_t kMinPlayers{ 2u };
	inline static constexpr uint8_t kMaxPlayers{ 5u };

private:
	std::string m_lobbyId;
	unsigned m_hostId;

	std::unordered_map<unsigned, PlayerInfo> m_players;
	std::unordered_map<unsigned, std::chrono::steady_clock::time_point> m_lastPingPlayer;
	unsigned m_lastMessageId{ 0u };
	std::deque<ChatMessage> m_chatBuffer;
	std::atomic<bool> m_gameStarted{ false };
	GameResult m_gameResult{ GameResult::Ongoing };
	mutable std::mutex m_mutex;

	std::unique_ptr<Table> m_table;
};