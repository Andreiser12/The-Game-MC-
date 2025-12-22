#include <iostream>
#include <filesystem>
#include <thread>
#include <memory>
#include <cpr/cpr.h>
#include <crow.h>

import Table;
import DatabaseManager;
import LobbyRoutes;
import LobbyManager;
import AuthenticationRoutes;

int main()
{	
	DatabaseManager database{ "gameDB.db" };
 
	crow::SimpleApp app;
	LobbyRoutes lobbyRoutes(LobbyManager::Instance(), database);
	lobbyRoutes.Register(app);
	AuthenticationRoutes auth(database);
	auth.Register(app);
	auth.Login(app);
	auth.Logout(app);
	auth.Users(app);
	app.port(18080).multithreaded().run();
	return 0;
}