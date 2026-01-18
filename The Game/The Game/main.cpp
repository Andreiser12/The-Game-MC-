#include <cpr/cpr.h>
#include <crow.h>

import Table;
import DatabaseManager;
import LobbyRoutes;
import LobbyManager;
import AuthenticationRoutes;

static inline const std::string SERVER_IP_ADDRESS{ "192.168.1.131" };
static inline constexpr uint16_t SERVER_PORT{ 18080 };

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
	auth.Profile(app);
	auth.DeleteAccount(app);
	auth.GetAvatar(app);
	auth.UpdateAvatar(app);
	app.bindaddr(SERVER_IP_ADDRESS).port(SERVER_PORT).multithreaded().run();
	return 0;
}