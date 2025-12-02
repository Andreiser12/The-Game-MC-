#pragma once
#include <string>
#include <optional>
#include <cpr/cpr.h>


class ApiClient
{
public:
	ApiClient(const std::string& serverURL);
	std::optional<std::string> CreateLobby();
	std::optional<std::string> JoinLobby(const std::string& lobbyCode);
	bool LeaveLobby(const std::string& lobbyId, unsigned playerId);
	std::optional<std::string> GetLobbyState(const std::string& lobbyId);

	bool StartMatch(unsigned lobbyId);


private:
	std::string m_serverURL;
	

};

