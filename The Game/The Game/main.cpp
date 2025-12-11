#include <iostream>
#include <filesystem>
#include <thread>
#include <memory>
#include <cpr/cpr.h>
#include <crow.h>

import Table;
import UserProfile;
import DatabaseManager;
import LobbyRoutes;
import LobbyManager;
import AuthenticationRoutes;

int main()
{	
	Table table(3);

	DatabaseManager database{ "gameDB.db" };

	crow::SimpleApp app;
	LobbyRoutes lobbyRoutes(LobbyManager::Instance(), database);
	lobbyRoutes.Register(app);
	AuthenticationRoutes auth(database);
	auth.Register(app);
	auth.Login(app);
	auth.Users(app);
	app.port(18080).multithreaded().run();
	return 0;
}