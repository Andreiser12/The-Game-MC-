module;

#include "crow.h"
#include <string>
#include <cstdint>

export module LobbyRoutes;

//import <crow.h>;
import LobbyManager;
import Lobby;

export class LobbyRoutes
{
public:
	LobbyRoutes(LobbyManager& manager);
	void Register(crow::SimpleApp& app);


private:
	crow::response CreateLobby(const crow::request& request);
	crow::response JoinLobby(const crow::request& request, const std::string& lobbyId);
	crow::response LeaveLobby(const crow::request& request, const std::string& lobbyId);
	crow::response GetLobbyState(const std::string& lobbyId);
	crow::response StartLobby(const crow::request& request, const std::string& lobbyId);

private:
	LobbyManager& m_manager;
};