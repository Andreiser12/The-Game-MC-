import AuthenticationRoutes;
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
