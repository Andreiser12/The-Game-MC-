#pragma once
#include "crow.h"
#include <cpr/cpr.h>
#include <string>
#include <memory>
#include <optional>

class ApiClient
{
public:
	ApiClient(const std::string& serverURL);
	std::optional<std::string> CreateLobby(const std::string& username);
	std::optional<unsigned> JoinLobby(const std::string& lobbyId,
		const std::string& username);
	bool LeaveLobby(const std::string& lobbyId, unsigned playerId);
	std::optional<std::string> GetLobbyState(const std::string& lobbyId);

	bool StartMatch(const std::string& lobbyId, unsigned playerId);

	void SetUsername(const std::string& username);
	void SetPlayerId(unsigned playerId);

	const std::string& GetUsername() const;
	unsigned GetPlayerId() const;

private:
	ApiClient() = delete;

private:
	std::string m_serverURL;
	std::string m_username;
	unsigned m_playerId;
};

