#include <iostream>
#include <filesystem>
#include <thread>
#include <memory>
#include <cpr/cpr.h>
#include <crow.h>

import Table;
import Menu;
import UserProfile;
import DatabaseManager;
import LobbyRoutes;
import LobbyManager;

void TestConnection(crow::SimpleApp& app, DatabaseManager &database)
{
	CROW_ROUTE(app, "/users")([&database]() {
		std::vector<crow::json::wvalue> usersJson;
		auto users = database.GetUsers();
		for (const auto& user : users)
		{
			usersJson.push_back(crow::json::wvalue{
				{"id", user.GetID()},
				{"username", user.GetUsername()}
				});
		}
		return crow::json::wvalue{ usersJson };
		});
	CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::Post)(
		[&database](const crow::request& req)
		{
			auto body = crow::json::load(req.body);
			if (!body)
				return crow::response(400, "Invalid JSON");
			std::string username = body["username"].s();

			auto user = database.GetUserByName(username);
			if (user) {
				crow::json::wvalue res;
				res["status"] = "ok";
				res["id"] = user->GetID();
				res["username"] = user->GetUsername();
				return crow::response{ res };
			}
			else
			{
				crow::json::wvalue res;
				res["status"] = "fail";
				res["message"] = "User not found";
				return crow::response{ res };
			}
		});
	CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::Post)(
		[&database](const crow::request& req)
		{
			auto body = crow::json::load(req.body);
			if (!body)
				return crow::response(400, "Invalid JSON");
			std::string username = body["username"].s();

			auto user = database.GetUserByName(username);
			if (user) {
				crow::json::wvalue res;
				res["status"] = "fail";
				res["message"] = "Username already exists!";
				return crow::response{ res };
			}
			DB::User newUser(username);
			database.AddUser(newUser);
			auto insertedUser = database.GetUserByName(username);
			crow::json::wvalue res;
			res["status"] = "ok";
			res["id"] = insertedUser->GetID();
			res["username"] = insertedUser->GetUsername();
			return crow::response{ res };
		});
	app.port(18080).multithreaded().run();
}

int main()
{	
	Table table(3);

	DatabaseManager database{ "gameDB.db" };

	crow::SimpleApp app;
	LobbyRoutes lobbyRoutes(LobbyManager::Instance(), database);
	lobbyRoutes.Register(app);
	TestConnection(app, database);

	return 0;
}