import LobbyRoutes;
#include "crow.h"

LobbyRoutes::LobbyRoutes(LobbyManager& manager) : m_manager{manager} {}

void LobbyRoutes::Register(crow::SimpleApp& app)
{
	CROW_ROUTE(app, "/lobby/create").methods("POST"_method)
		([this](const crow::request& request) {
		return this->CreateLobby(request);
		});

	CROW_ROUTE(app, "/lobby/join/<uint>").methods("POST"_method)
		([this](const crow::request& req, uint32_t lobbyId) {
		return this->JoinLobby(req, lobbyId);
		});

	CROW_ROUTE(app, "/lobby/leave/<uint>").methods("POST"_method)
		([this](const crow::request& req, uint32_t lobbyId) {
		return this->LeaveLobby(req, lobbyId);
		});

	CROW_ROUTE(app, "/lobby/state/<uint>").methods("POST"_method)
		([this](uint32_t lobbyId) {
		return this->GetLobbyState(lobbyId);
		});

	CROW_ROUTE(app, "/lobby/start/<uint>").methods("POST"_method)
		([this](const crow::request& req, uint32_t lobbyId) {
		return this->StartLobby(req, lobbyId);
		});

}

crow::response LobbyRoutes::CreateLobby(const crow::request& request)
{
	return crow::response();
}

crow::response LobbyRoutes::JoinLobby(const crow::request& request, uint32_t lobbyId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby)
		return crow::response(404, "Lobby not found");

	crow::json::wvalue data;
	data["message"] = "joined";
	data["lobbyId"] = lobbyId;

	return crow::response(data);
}

crow::response LobbyRoutes::LeaveLobby(const crow::request& request, uint32_t lobbyId)
{
	auto body = crow::json::load(request.body);
	if (!body)
		return crow::response(400, "Invalid JSON");

	uint32_t playerId = body["player_id"].i();

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby)
		return crow::response(404, "Lobby not found");

	return crow::response(200, "left");
}

crow::response LobbyRoutes::GetLobbyState(uint32_t lobbyId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby)
		return crow::response(404, "Lobby not found");

	crow::json::wvalue data;
	/*data["id"] = lobby->GetId();
	data["minPlayers"] = lobby->GetMinPlayers();
	data["maxPlayers"] = lobby->GetMaxPlayers();*/
	data["id"] = lobbyId;
	data["minPlayers"] = "2";
	data["maxPlayers"] = "5";

	return crow::response(data);
}

crow::response LobbyRoutes::StartLobby(const crow::request& request, uint32_t lobbyId)
{
	auto body = crow::json::load(request.body);
	if (!body)
		return crow::response(400, "Invalid JSON");

	uint32_t playerId = body["player_id"].i();

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby)
		return crow::response(404, "Lobby not found");

	return crow::response("STARTED");
}
