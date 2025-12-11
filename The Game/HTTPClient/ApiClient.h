#pragma once
#include "crow.h"
#include <cpr/cpr.h>
#include <string>
#include <memory>
#include <vector>
#include <optional>

struct ChatMessage
{
	unsigned id;
	std::string username;
	std::string message;

	ChatMessage(unsigned i, std::string u, std::string m) : 
		id(i), username(std::move(u)), message(std::move(m)) {}
};

class ApiClient
{
public:
	ApiClient(const std::string& serverURL);
	std::optional<std::string> CreateLobby(const std::string& username);
	bool JoinLobby(const std::string& lobbyId,
		const std::string& username);
	bool LeaveLobby(const std::string& lobbyId, unsigned playerId);
	std::optional<std::string> GetLobbyState(const std::string& lobbyId);

	bool StartMatch(const std::string& lobbyId, unsigned playerId);
	bool SendChatMessage(const std::string& message);
	std::optional<std::vector<ChatMessage>> GetNewMessages(unsigned afterId);

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
	std::string m_serverURL;
	std::string m_username;
	std::string m_lobbyId;
	unsigned m_playerId;
};

