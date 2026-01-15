#pragma once

#include "UserProfile.h"
#include "crow.h"
#include <cpr/cpr.h>
#include <memory>
#include <vector>
#include <array>
#include <optional>

enum class ClientEvent
{
	None,
	PlayerLeft,
	LobbyClosed,
	HostLeftLobby
};

enum class LoginResult
{
	Success,
	UserNotFound,
	AlreadyLoggedIn,
	NetworkError
};

enum class GameResult
{
	Ongoing,
	Won,
	Lost,
	NotEnoughPlayers
};

enum class LeaveMatchResult
{
	Success,
	NotInLobby,
	AlreadyLeft
};

struct ChatMessage
{
	unsigned id;
	std::string username;
	std::string message;

	ChatMessage(unsigned i, std::string u, std::string m) :
		id(i), username(std::move(u)), message(std::move(m)) {
	}
};

struct DeckState
{
	uint8_t deckId;
	uint8_t topCardId;
};

struct LobbyState
{
	std::string m_status;
	std::string m_message;
	unsigned m_hostId;
};

struct GameTableState
{
	int drawPileCount{ 0 };
};

struct EndTurnState
{
	unsigned playerId;
	bool canEndTurn;
	uint8_t cardsPlaced;
	uint8_t minRequiredCards;
};

struct TurnState
{
	bool isMyTurn;
	unsigned currentPlayerId;
};

struct PlayerState
{
	std::string username;
	bool isInMatch;
};

class ApiClient
{
public:
	ApiClient(const std::string& serverURL);

	LoginResult Login(const std::string& username);
	bool Logout();
	bool IsLoggedIn();
	std::optional<std::string> CreateLobby(const std::string& username);
	bool JoinLobby(const std::string& lobbyId,
		const std::string& username);
	bool LeaveLobby(const std::string& lobbyId, unsigned playerId);
	std::optional<LeaveMatchResult> LeaveMatch();
	std::optional<LobbyState> GetLobbyState(const std::string& lobbyId);
	std::optional<std::vector<PlayerState>> GetPlayerStates();

	bool StartMatch(const std::string& lobbyId, unsigned playerId);
	bool SendChatMessage(const std::string& message);
	std::optional<std::vector<ChatMessage>> GetNewMessages(unsigned afterId);
	bool PlayCard(uint8_t cardId, uint8_t deckId);

	bool SendPing(const std::string& lobbyId, unsigned playerId);
	void StartPingThread(const std::string& lobbyId, unsigned playerId);
	void StopPingThread();

	ClientEvent GetEvent() const;
	void ClearEvent();

	void StartLobbyWatcher();
	void StopLobbyWatcher();

	std::string GetUsername() const;
	unsigned GetPlayerId() const;
	std::string GetLobbyId() const;
	std::optional<UserProfile> GetUserProfile(unsigned playerId);
	std::optional<std::vector<uint8_t>> EndTurn();
	std::optional<TurnState> GetTurnState() const;
	std::optional<GameResult> ParseGameResult(const std::string& result) const;
	std::optional<GameResult> GetGameResult(const std::string& lobbyId) const;

	void StartNetworkLoop();
	void StopNetworkLoop();

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

	std::optional<std::vector<DeckState>> GetDeckState() const;
	std::optional<EndTurnState> GetEndTurnState() const;
	std::vector<std::string> GetLobbyPlayers(const std::string& lobbyId) const;
	std::optional<std::vector<uint8_t>> GetPlayerCards() const;
	std::optional<std::array<bool, 4>> GetValidDecks(uint8_t cardId) const;
	std::optional<bool> GetCanPlayMore(uint8_t remainingCards) const;

	void SetUsername(const std::string& username);
	void SetPlayerId(unsigned playerId);
	void SetLobbyId(const std::string& lobbyId);

	bool ReportGameLoss();

private:
	ApiClient() = delete;

private:
	std::string m_serverURL;

	mutable std::mutex m_mutex;
	std::string m_username;
	std::string m_lobbyId;
	unsigned m_playerId{ 0u };
	std::atomic<ClientEvent> m_event{ ClientEvent::None };
	std::atomic<bool> m_pingRunning{ false };
	std::atomic<unsigned> m_afterChatId{ 0u };

	std::jthread m_netThread;
	std::jthread m_pingThread;
	std::jthread m_lobbyWatcher;

	mutable std::mutex m_cacheMutex;
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
