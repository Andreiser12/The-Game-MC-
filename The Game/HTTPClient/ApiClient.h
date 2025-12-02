#pragma once
#include <string>
#include <optional>
#include <cpr/cpr.h>


class ApiClient
{
public:
	ApiClient(const std::string& serverURL);
	std::optional<std::string> CreateLobby();
	std::optional<std::string> JoinLobby(unsigned lobbyCode);
	bool LeaveLobby(unsigned lobbyId);
	std::optional<std::string> GetLobbyState(unsigned lobbyId);

	bool StartMatch(unsigned lobbyId);


private:
	std::string m_serverURL;
	

};

