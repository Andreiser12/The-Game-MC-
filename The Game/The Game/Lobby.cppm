export module Lobby;

import Table;
import ChatFilter;

import <stdint.h>;
import <unordered_map>;
import <deque>;
import <atomic>;
import <mutex>;
import <chrono>;
import <ranges>;
import <algorithm>;

export constexpr double kSecondsPerHour{ 3600.0 };

export enum class RemoveResult : uint8_t
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
	bool isSystem;
};

export struct DeckState
{
	uint8_t deckId;
	uint8_t topCardId;
	uint8_t size;
};

export struct EndTurnState
{
	unsigned playerId;
	uint8_t cardsPlaced;
	uint8_t minRequiredCards;
	bool canEndTurn;
};

export struct TurnState
{
	unsigned currentPlayerId;
	bool isMyTurn;
	std::string currentUsername;
};

export enum class GameResult : uint8_t
{
	Ongoing,
	Won,
	Lost,
	NotEnoughPlayers
};

export struct PlayerState
{
	unsigned playerId;
	std::string username;
	bool isInMatch;
};

export struct PlayerInfo
{
	unsigned playerId;
	std::string username;
	bool isConnected{ true };

	PlayerInfo(unsigned id, std::string_view name) : playerId{ id }, username{ name } {}
};

export class Lobby
{
public:
	explicit Lobby(std::string_view lobbyId, unsigned hostId, const std::string& username);

	~Lobby() = default;
	Lobby(const Lobby&) = delete;
	Lobby& operator =(const Lobby&) = delete;
	Lobby(Lobby&&) = delete;
	Lobby& operator =(Lobby&&) = delete;

	[[nodiscard]] bool AddPlayer(unsigned playerId, const std::string& playerUsername);
	[[nodiscard]] RemoveResult RemovePlayer(unsigned playerId);
	void MarkPlayerAsDisconnected(unsigned playerId) noexcept;
	[[nodiscard]] RemoveResult RemoveMatchPlayer(unsigned playerId);
	bool IsGameActive() const noexcept;

	void AddChatMessage(std::string_view username, std::string_view message, bool isSystem);
	[[nodiscard]] std::optional<std::vector<ChatMessage>> GetMessagesAfter(unsigned afterId) const;

	[[nodiscard]] bool StartGame(unsigned playerId);
	[[nodiscard]] bool HasGameStarted() const noexcept
	{
		return m_gameStarted.load(std::memory_order_acquire);
	}
	[[nodiscard]] bool PlayCard(unsigned playerId, uint8_t cardId, uint8_t deckId);
	[[nodiscard]] std::optional<std::vector<uint8_t>> EndTurn(unsigned playerId);
	void UpdateGameResult() noexcept;

	[[nodiscard]] std::optional<std::vector<DeckState>> GetDecksState() const;
	[[nodiscard]] std::optional<size_t> GetDrawPileCount() const;
	[[nodiscard]] std::optional<EndTurnState> GetEndTurnState(unsigned playerId) const;
	[[nodiscard]] std::optional<TurnState> GetTurnState(unsigned playerId) const;
	[[nodiscard]] std::optional<GameResult> GetGameResult() const;
	[[nodiscard]] double GetMatchDurationHours() const;
	bool StatsAlreadyUpdated() const noexcept;
	void MarkStatsUpdated() noexcept;
	[[nodiscard]] std::optional<std::vector<uint8_t>> GetPlayerCards(unsigned playerId) const;
	[[nodiscard]] std::optional<std::array<bool, 4>> GetValidDecks(uint8_t cardId) const;
	[[nodiscard]] std::optional<std::vector<PlayerState>> GetPlayerStates() const;
	[[nodiscard]] std::optional<bool> GetCanPlayMore(unsigned playerId,
		uint8_t remainingCards) const;
	[[nodiscard]] std::optional<std::vector<std::pair<uint8_t, uint8_t>>> GetAICards(unsigned playerId,
		uint8_t cardsRequired);

	void UpdatePlayerPing(unsigned playerId) noexcept;
	std::vector<unsigned> GetInactivePlayers(std::chrono::seconds timeout) const;

	[[nodiscard]] bool IsFull() const noexcept;
	[[nodiscard]] size_t GetPlayerCount() const noexcept;

	[[nodiscard]] const std::unordered_map<unsigned, PlayerInfo>& GetPlayers() const;
	[[nodiscard]] bool IsPlayerHost(unsigned playerId) const;
	unsigned GetHostId() const;

	void SetGameLost();

private:
	Lobby() = delete;
	void TrimChatBuffer() noexcept;

public:
	inline static constexpr size_t kMaxChatMessages{ 100 };
	inline static constexpr uint8_t kMinPlayers{ 2u };
	inline static constexpr uint8_t kMaxPlayers{ 5u };
	inline static constexpr uint8_t kDeckCount{ 4u };

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
	std::chrono::steady_clock::time_point m_gameStartTime;
	bool m_statsUpdated{ false };
};