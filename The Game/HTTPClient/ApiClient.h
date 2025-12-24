#pragma once
#include "crow.h"
#include <cpr/cpr.h>
#include <string>
#include <memory>
#include <vector>
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

struct ChatMessage
{
	unsigned id;
	std::string username;
	std::string message;

	ChatMessage(unsigned i, std::string u, std::string m) : 
		id(i), username(std::move(u)), message(std::move(m)) {}
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
	int drawPileCount = 0;
};

struct EndTurnState
{
	bool canEndTurn;
	uint8_t cardsPlaced;
	uint8_t minRequiredCards;
};

struct TurnState
{
	bool isMyTurn;
	unsigned currentPlayerId;
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
	std::optional<LobbyState> GetLobbyState(const std::string& lobbyId);

	bool StartMatch(const std::string& lobbyId, unsigned playerId);
	bool SendChatMessage(const std::string& message);
	std::optional<std::vector<ChatMessage>> GetNewMessages(unsigned afterId);
	bool PlayCard(uint8_t cardId, uint8_t deckId);
	std::optional<std::vector<DeckState>> GetDeckState();
	std::optional<EndTurnState> GetEndTurnState();

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
	std::vector<std::string> GetLobbyPlayers(const std::string& lobbyId) const;
	std::optional<std::vector<uint8_t>> GetPlayerCards() const;
	std::optional<std::vector<uint8_t>> EndTurn();
	std::optional<TurnState> GetTurnState() const;

	void StartNetworkLoop();
	void StopNetworkLoop();

	std::vector<std::string> SnapshotPlayers() const;
	std::optional<LobbyState> SnapshotLobbyState() const;
	std::optional<std::vector<ChatMessage>> SnapshotNewChat();
	std::optional<std::vector<DeckState>> SnapshotDecks() const;
	std::optional<GameTableState> GetTableState(const std::string& lobbyId) const;
	std::optional<EndTurnState> SnapshotEndTurnState() const;
	std::optional<GameTableState> SnapshotGameTableState() const;
	std::optional<TurnState> SnapshotTurnState() const;

	void SetUsername(const std::string& username);
	void SetPlayerId(unsigned playerId);
	void SetLobbyId(const std::string& lobbyId);

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

	std::atomic<unsigned> m_afterChatId{ 0u };
};
