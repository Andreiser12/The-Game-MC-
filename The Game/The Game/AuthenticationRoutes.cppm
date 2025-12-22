module;
#include "crow.h"
export module AuthenticationRoutes;

import SessionManager;
import DatabaseManager;
import DatabaseStructures;

export class AuthenticationRoutes
{
	public:
		explicit AuthenticationRoutes(DatabaseManager& database);
		void Register(crow::SimpleApp& app);
		void Login(crow::SimpleApp& app);
		void Users(crow::SimpleApp& app);
		
	private:
		crow::response HandleRegister(const crow::request& req);
		crow::response HandleLogin(const crow::request& req);
		crow::response HandleUsers();
	private:
		DatabaseManager& m_database;
};