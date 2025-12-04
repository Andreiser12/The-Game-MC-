import LobbyRoutes;
#include "crow.h"

LobbyRoutes::LobbyRoutes(LobbyManager& manager) : m_manager{manager} {}

void LobbyRoutes::Register(crow::SimpleApp& app)
{
	CROW_ROUTE(app, "/lobby/create").methods("POST"_method)
		([this](const crow::request& req) {
		return this->CreateLobby(req);
		});

	CROW_ROUTE(app, "/lobby/join").methods("POST"_method)
		([this](const crow::request& req) {
		return this->JoinLobby(req);
		});

	CROW_ROUTE(app, "/lobby/leave").methods("POST"_method)
		([this](const crow::request& req) {
		return this->LeaveLobby(req);
		});

	CROW_ROUTE(app, "/lobby/info/<string>").methods("GET"_method)
		([this](const std::string& lobbyId) {
		return this->GetLobbyState(lobbyId);
		});

	CROW_ROUTE(app, "/lobby/start").methods("POST"_method)
		([this](const crow::request& req) {
		return this->StartLobby(req);
		});
}

crow::response LobbyRoutes::CreateLobby(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	std::string username = body["username"].s();
	std::string lobbyId;
	const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ01234556789";
	for (int i = 0; i < 6; i++)
	{
		lobbyId += charset[rand() % sizeof(charset) - 1];
	}
	auto lobby = m_manager.CreateLobby(lobbyId, 2, 5);
	if (!lobby) return crow::response(500, "Failed to create lobby!");
}

crow::response LobbyRoutes::JoinLobby(const crow::request& request)
{
	std::string lobbyId;
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby)
		return crow::response(404, "Lobby not found");

	crow::json::wvalue data;
	data["message"] = "joined";
	data["lobbyId"] = lobbyId;

	return crow::response(data);
}

crow::response LobbyRoutes::LeaveLobby(const crow::request& request)
{
	std::string lobbyId;
	auto body = crow::json::load(request.body);
	if (!body)
		return crow::response(400, "Invalid JSON");

	uint32_t playerId = body["player_id"].i();

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby)
		return crow::response(404, "Lobby not found");

	return crow::response(200, "left");
}

crow::response LobbyRoutes::GetLobbyState(const std::string& lobbyId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby)
		return crow::response(404, "Lobby not found");

	crow::json::wvalue data;
	data["id"] = lobbyId;
	data["minPlayers"] = "2";
	data["maxPlayers"] = "5";

	return crow::response(data);
}

crow::response LobbyRoutes::StartLobby(const crow::request& request)
{
	std::string lobbyId;
	auto body = crow::json::load(request.body);
	if (!body)
		return crow::response(400, "Invalid JSON");

	uint32_t playerId = body["player_id"].i();

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby)
		return crow::response(404, "Lobby not found");

	return crow::response("STARTED");
}