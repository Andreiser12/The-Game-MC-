#pragma once
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

private:
	std::string m_serverURL;
	
};

