import AuthenticationRoutes;
import SessionManager;
#include <crow.h>

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

void AuthenticationRoutes::Users(crow::SimpleApp& app)
{
	CROW_ROUTE(app, "/users")([this]() {
		return this->HandleUsers();
	});
}

crow::response AuthenticationRoutes::HandleRegister(const crow::request& req)
{
	auto body = crow::json::load(req.body);
	if (!body)
		return crow::response(400, "Invalid JSON");
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
	if (!body)
		return crow::response(400, "Invalid JSON");
	std::string username = body["username"].s();

	auto userOptional = m_database.GetUserByName(username);
	if (!userOptional) 
	{
		crow::json::wvalue result;
		result["status"] = "fail";
		result["message"] = "User not found";
		return crow::response{ result };
	}
	
	unsigned playerId = userOptional->GetID();
	auto& sessionManager = SessionManager::Instance();

	if (sessionManager.IsLoggedIn(playerId))
	{
		crow::json::wvalue result;
		result["status"] = "fail";
		result["message"] = "User not found!";
		return crow::response{ result };
	}

	sessionManager.Login(playerId);

	crow::json::wvalue result;
	result["status"] = "ok";
	result["id"] = userOptional->GetID();
	result["username"] = userOptional->GetUsername();
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
