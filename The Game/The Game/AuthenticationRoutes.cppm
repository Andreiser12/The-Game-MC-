module;
#include "crow.h"

export module AuthenticationRoutes;

import DatabaseManager;
import DatabaseStructures;
import <filesystem>;
import <fstream>;
import <sstream>;
export class AuthenticationRoutes
{
public:
	explicit AuthenticationRoutes(DatabaseManager& database);
	void Register(crow::SimpleApp& app);
	void Login(crow::SimpleApp& app);
	void Logout(crow::SimpleApp& app);
	void Users(crow::SimpleApp& app);
	void Profile(crow::SimpleApp& app);
	void GetAvatar(crow::SimpleApp& app);
	void UpdateAvatar(crow::SimpleApp& app);
	void DeleteAccount(crow::SimpleApp& app);
	
private:
	crow::response HandleRegister(const crow::request& req);
	crow::response HandleLogin(const crow::request& req);
	crow::response HandleLogout(const crow::request& req);
	crow::response HandleUsers();
	crow::response HandleUserProfile(unsigned playerId);
	crow::response HandleGetAvatar(const std::string& filename);
	void HandleUpdateAvatar(unsigned playerId, const crow::request& req, crow::response& response);
	crow::response HandleDeleteAccount(unsigned playerId);

private:
	DatabaseManager& m_database;
};