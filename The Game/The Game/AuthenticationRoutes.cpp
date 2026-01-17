import AuthenticationRoutes;
import SessionManager;

#include "crow.h"

AuthenticationRoutes::AuthenticationRoutes(DatabaseManager& database)
	: m_database{ database } { }

void AuthenticationRoutes::Register(crow::SimpleApp& app)
{
	CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::Post)(
		[this](const crow::request& req)
		{
			return this->HandleRegister(req);
		});
}

void AuthenticationRoutes::Login(crow::SimpleApp& app)
{
	CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::Post)(
		[this](const crow::request& req)
		{
			return this->HandleLogin(req);
		});
}

void AuthenticationRoutes::Logout(crow::SimpleApp& app)
{
	CROW_ROUTE(app, "/logout").methods(crow::HTTPMethod::Post)(
		[this](const crow::request& req)
		{
			return this->HandleLogout(req);
		});
}

void AuthenticationRoutes::Users(crow::SimpleApp& app)
{
	CROW_ROUTE(app, "/users")([this]() {
		return this->HandleUsers();
	});
}

void AuthenticationRoutes::Profile(crow::SimpleApp& app)
{
	CROW_ROUTE(app, "/users/<uint>/profile").methods("GET"_method)
		([this](unsigned playerId)
		{
			return this->HandleUserProfile(playerId);
		});
}

void AuthenticationRoutes::DeleteAccount(crow::SimpleApp& app)
{
	CROW_ROUTE(app, "/users/<uint>").methods(crow::HTTPMethod::Delete)
		([this](unsigned playerId)
			{
				return this->HandleDeleteAccount(playerId);
			});
}

crow::response AuthenticationRoutes::HandleRegister(const crow::request& req)
{
	auto body = crow::json::load(req.body);
	if (!body) return crow::response(400, "Invalid JSON");

	std::string username = body["username"].s();

	auto user = m_database.GetUserByName(username);
	if (user) {
		crow::json::wvalue res;
		res["status"] = "fail";
		res["message"] = "Username already exists!";
		return crow::response{ res };
	}
	DB::User newUser(username);
	m_database.AddUser(newUser);
	auto insertedUser = m_database.GetUserByName(username);
	crow::json::wvalue res;
	res["status"] = "ok";
	res["id"] = insertedUser->GetID();
	res["username"] = insertedUser->GetUsername();
	return crow::response{ res };
}

crow::response AuthenticationRoutes::HandleLogin(const crow::request& req)
{
	auto body = crow::json::load(req.body);
	if (!body) return crow::response(400, "Invalid JSON");

	std::string username = body["username"].s();

	auto userOpt = m_database.GetUserByName(username);
	if (!userOpt) 
	{
		crow::json::wvalue result;
		result["status"] = "fail";
		result["message"] = "User not found";
		return crow::response{ result };
	}
	
	unsigned playerId{ userOpt->GetID() };
	auto& sessionManager = SessionManager::Instance();

	if (sessionManager.IsLoggedIn(playerId))
	{
		crow::json::wvalue result;
		result["status"] = "fail";
		result["message"] = "You're already logged in!";
		return crow::response{ result };
	}

	sessionManager.Login(playerId);

	crow::json::wvalue result;
	result["status"] = "ok";
	result["id"] = userOpt->GetID();
	result["username"] = userOpt->GetUsername();
	return crow::response{ result };
}

crow::response AuthenticationRoutes::HandleLogout(const crow::request& req)
{
	auto body = crow::json::load(req.body);
	if (!body) return crow::response(400, "Invalid JSON");
	 
	unsigned playerId = static_cast<unsigned>(body["playerId"].i());
	auto& sessionManager = SessionManager::Instance();

	sessionManager.Logout(playerId);

	crow::json::wvalue result;
	result["status"] = "ok";
	return crow::response{ result };
}

crow::response AuthenticationRoutes::HandleUsers()
{
	std::vector<crow::json::wvalue> usersJson;
	auto users = m_database.GetUsers();
	for (const auto& user : users)
	{
		usersJson.push_back(crow::json::wvalue{
			{"id", user.GetID()},
			{"username", user.GetUsername()}
			});
	}
	return crow::json::wvalue{ usersJson };
}

crow::response AuthenticationRoutes::HandleUserProfile(unsigned playerId)
{
	auto userOpt = m_database.GetUserByID(playerId);
	if (!userOpt) return crow::response(404, "User not found");

	const auto& user = userOpt.value();

	crow::json::wvalue data;
	data["username"] = user.GetUsername();
	data["gamesPlayed"] = user.GetGamesPlayed();
	data["gamesWon"] = user.GetGamesWon();
	data["hoursPlayed"] = user.GetHoursPlayed();
	return crow::response(200, data);
}

crow::response AuthenticationRoutes::HandleDeleteAccount(unsigned playerId)
{
	auto& sessionManager = SessionManager::Instance();
	if (sessionManager.IsLoggedIn(playerId))
		sessionManager.Logout(playerId);
	if (!m_database.DeleteUser(playerId))
		return crow::response(404, "User not found");

	crow::json::wvalue result;
	result["status"] = "ok";
	result["message"] = "Account deleted";
	return crow::response(200, result);
}
