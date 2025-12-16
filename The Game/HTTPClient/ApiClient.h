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
struct ChatMessage
{
	unsigned id;
	std::string username;
	std::string message;

	ChatMessage(unsigned i, std::string u, std::string m) : 
		id(i), username(std::move(u)), message(std::move(m)) {}
};

struct LobbyState {
	std::string m_status;
	std::string m_message;
	unsigned m_hostId;
};

class ApiClient
{
public:
	ApiClient(const std::string& serverURL);
	std::optional<std::string> CreateLobby(const std::string& username);
	bool JoinLobby(const std::string& lobbyId,
		const std::string& username);
	bool LeaveLobby(const std::string& lobbyId, unsigned playerId);
	std::optional<LobbyState> GetLobbyState(const std::string& lobbyId);

	bool StartMatch(const std::string& lobbyId, unsigned playerId);
	bool SendChatMessage(const std::string& message);
	std::optional<std::vector<ChatMessage>> GetNewMessages(unsigned afterId);

	bool SendPing(const std::string& lobbyId, unsigned playerId);
	void StartPingThread(const std::string& lobbyId, unsigned playerId);
	void StopPingThread();

	ClientEvent GetEvent() const;
	void ClearEvent();

	void StartLobbyWatcher();
	void StopLobbyWatcher();

	void SetUsername(const std::string& username);
	void SetPlayerId(unsigned playerId);
	void SetLobbyId(const std::string& lobbyId);
	
	const std::string& GetUsername() const;
	unsigned GetPlayerId() const;
	const std::string& GetLobbyId() const;
	std::vector<std::string> GetLobbyPlayers(const std::string& lobbyId) const;


private:
	ApiClient() = delete;

private:
	mutable std::mutex m_mutex;
	std::string m_serverURL;
	std::string m_username;
	std::string m_lobbyId;
	unsigned m_playerId;
	std::atomic<bool> m_pingRunning{ false };
	std::jthread m_pingThread;
	std::atomic<ClientEvent> m_event{ ClientEvent::None };
	std::jthread m_lobbyWatcher;
};

