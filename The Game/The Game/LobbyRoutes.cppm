module;

#include "crow.h"
#include <string>
#include <cstdint>

export module LobbyRoutes;

import LobbyManager;
import Lobby;

export class LobbyRoutes
{
public:
	LobbyRoutes(LobbyManager& manager);
	void Register(crow::SimpleApp& app);


private:
	crow::response CreateLobby(const crow::request& request);
	crow::response JoinLobby(const crow::request& request);
	crow::response LeaveLobby(const crow::request& request);
	crow::response GetLobbyState(const std::string& lobbyId);
	crow::response StartLobby(const crow::request& request);

private:
	LobbyManager& m_manager;
};