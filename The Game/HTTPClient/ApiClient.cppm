module;
#define NOMINMAX
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <array>
#include <mutex>
#include <atomic>
#include <thread>
#include <unordered_map>
#include <fstream>

#include <cpr/cpr.h>
#include "crow.h"

export module ApiClient;

import UserProfile;

export enum class ClientEvent
{
	None,
	PlayerLeft,
	LobbyClosed,
	HostLeftLobby,
	MatchStarted
};

export enum class LoginResult
{
	Success,
	UserNotFound,
	AlreadyLoggedIn,
	NetworkError
};

export enum class GameResult
{
	Ongoing,
	Won,
	Lost,
	NotEnoughPlayers
};

export enum class LeaveMatchResult
{
	Success,
	NotInLobby,
	AlreadyLeft
};

export struct ChatMessage
{
	unsigned id;
	std::string username;
	std::string message;
	bool isSystem;

	ChatMessage(unsigned i, std::string u, std::string m, bool system) :
		id(i),
		username(std::move(u)),
		message(std::move(m)),
		isSystem(system)
	{}
};

export struct DeckState
{
	uint8_t deckId;
	uint8_t topCardId;
	uint8_t size;
};

export struct LobbyState
{
	std::string m_status;
	std::string m_message;
	unsigned m_hostId;
};

export struct GameTableState
{
	int drawPileCount{ 0 };
};

export struct EndTurnState
{
	unsigned playerId;
	bool canEndTurn;
	uint8_t cardsPlaced;
	uint8_t minRequiredCards;
};

export struct TurnState
{
	bool isMyTurn;
	unsigned currentPlayerId;
	std::string currentUsername;
};

export struct PlayerState
{
	unsigned playerId;
	std::string username;
	bool isInMatch;
};

export enum class JoinLobbyResult
{
	Success,
	LobbyNotFound,
	LobbyFull,
	GameAlreadyStarted,
	NetworkError
};

export struct LobbyInfo
{
	std::string lobbyId;
	std::string status;
	uint8_t currentPlayers;
	uint8_t maxPlayers;
	unsigned hostId;
	bool isGameStarted;
};

export class ApiClient
{
public:
	explicit ApiClient(const std::string& serverURL);

	ApiClient(const ApiClient&) = delete;
	ApiClient& operator= (const ApiClient&) = delete;

	ApiClient(ApiClient&&) noexcept = default;
	ApiClient& operator= (ApiClient&&) noexcept = default;

	LoginResult Login(const std::string& username);
	bool Logout();
	bool IsLoggedIn();
	bool DeleteAccount();

	std::optional<std::string> CreateLobby(const std::string& username) const;
	JoinLobbyResult JoinLobbyValidated(const std::string& lobbyId, const std::string& username);
	bool LeaveLobby(const std::string& lobbyId, unsigned playerId);
	std::optional<crow::json::rvalue> GetLobbyInfo(const std::string& lobbyId) const;
	std::vector<std::string> GetLobbyPlayers(const std::string& lobbyId) const;

	bool StartMatch(const std::string& lobbyId, unsigned playerId) const;
	std::optional<LeaveMatchResult> LeaveMatch();
	std::optional<std::vector<PlayerState>> GetPlayerStates() const;
	bool ReportGameLoss();

	bool SendChatMessage(const std::string& message, bool isSystem) const;
	std::optional<std::vector<ChatMessage>> GetNewMessages(unsigned afterId);

	bool PlayCard(uint8_t cardId, uint8_t deckId) const;
	std::optional<std::vector<uint8_t>> EndTurn();
	std::optional<std::vector<DeckState>> GetDecksState() const;
	std::optional<EndTurnState> GetEndTurnState() const;
	std::optional<std::vector<uint8_t>> GetPlayerCards() const;
	std::optional<std::array<bool, 4>> GetValidDecks(uint8_t cardId) const;
	std::optional<bool> GetCanPlayMore(uint8_t remainingCards) const;
	std::optional<GameResult> GetGameResult(const std::string& lobbyId) const;
	std::optional<TurnState> GetTurnState() const;
	std::optional<std::vector<std::pair<uint8_t, uint8_t>>> GetAICards(uint8_t cardsRequired) const;

	bool SendPing(const std::string& lobbyId, unsigned playerId) const;
	void StartNetworkLoop();
	void StopNetworkLoop();

	ClientEvent GetEvent() const;
	void ClearEvent();

	std::string GetUsername() const;
	unsigned GetPlayerId() const;
	std::string GetLobbyId() const;
	std::optional<UserProfile> GetUserProfile(unsigned playerId) const;
	bool DownloadAvatar(unsigned playerId, const std::string& localPath);
	bool UpdateAvatar(const std::string& imagePath);
	bool IsGameActive() const;

	void SetUsername(const std::string& username);
	void SetPlayerId(unsigned playerId);
	void SetLobbyId(const std::string& lobbyId);
	void SetGameActive(bool active);

	std::vector<std::string> SnapshotPlayers() const;
	std::optional<LobbyState> SnapshotLobbyState() const;
	std::optional<std::array<bool, 4>> SnapshotValidDecks(uint8_t cardId) const;
	std::optional<std::vector<ChatMessage>> SnapshotNewChat();
	std::optional<std::vector<DeckState>> SnapshotDecks() const;
	std::optional<GameTableState> GetTableState(const std::string& lobbyId) const;
	std::optional<EndTurnState> SnapshotEndTurnState() const;
	std::optional<GameTableState> SnapshotGameTableState() const;
	std::optional<TurnState> SnapshotTurnState() const;
	std::optional<GameResult> SnapshotGameResult() const;
	std::optional<std::vector<PlayerState>> SnapshotPlayerStates() const;
	std::optional<bool> SnapshotCanPlayMore() const;

	std::optional<GameResult> ParseGameResult(const std::string& result) const;

private:
	ApiClient() = delete;

private:
	std::string m_serverURL;

	mutable std::mutex m_mutex;
	mutable std::mutex m_cacheMutex;

	std::string m_username;
	std::string m_lobbyId;
	unsigned m_playerId{ 0u };

	std::atomic<ClientEvent> m_event{ ClientEvent::None };
	std::atomic<bool> m_isGameActive{ false };
	std::atomic<unsigned> m_afterChatId{ 0u };

	std::jthread m_netThread;

	std::vector<std::string> m_cachedPlayers;
	std::optional<LobbyState> m_cachedLobby;
	std::optional<std::vector<ChatMessage>> m_cachedChat;
	std::optional<std::vector<DeckState>> m_cachedDecks;
	std::optional<GameTableState> m_cachedGameTableState;
	std::optional<EndTurnState> m_cachedEndTurnState;
	std::optional<TurnState> m_cachedTurnState;
	std::optional<GameResult> m_cachedGameResult;
	std::optional<std::vector<PlayerState>> m_cachedPlayerStates;
	std::unordered_map<uint8_t, std::optional<std::array<bool, 4>>> m_cachedValidDecks;
	std::optional<bool> m_canPlayMore{ true };
};
