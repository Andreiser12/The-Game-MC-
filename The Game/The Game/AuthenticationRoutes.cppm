module;
#include "crow.h";
export module AuthenticationRoutes;

import DatabaseManager;
import DatabaseStructures;

export class AuthenticationRoutes
{
	public:
		explicit AuthenticationRoutes(DatabaseManager& database);
		void Register(crow::SimpleApp& app);
		void Login(crow::SimpleApp& app);
	private:
		crow::response HandleRegister(const crow::request& req);
	private:
		DatabaseManager& m_database;
};