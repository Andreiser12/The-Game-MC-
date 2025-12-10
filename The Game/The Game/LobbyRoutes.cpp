import LobbyRoutes;
#include "crow.h"

constexpr static uint8_t K_LOBBY_ID_LENGTH{ 6u };

inline static std::string GenerateLobbyId()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dist(0, 35);
	const std::string charset{ "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };
	std::string lobbyId;
	lobbyId.reserve(K_LOBBY_ID_LENGTH);
	for (uint8_t i = 0u; i < K_LOBBY_ID_LENGTH; i++)
	{
		lobbyId += charset[dist(gen)];
	}
	return lobbyId;
}

LobbyRoutes::LobbyRoutes(LobbyManager& manager, DatabaseManager& db)
	: m_manager{manager},
	m_db{db} {}

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
		return this->StartMatch(req);
		});
}

crow::response LobbyRoutes::CreateLobby(const crow::request& request)
{
	auto json = crow::json::load(request.body);
	if (!json) return crow::response(400, "Invalid JSON");

	const std::string lobbyId = GenerateLobbyId();
	auto lobby = m_manager.CreateLobby(lobbyId, json["playerId"].i(), json["username"].s());
	if (!lobby) return crow::response(500, "Failed to create lobby!");

	crow::json::wvalue data;
	data["lobbyId"] = lobbyId;
	data["status"] = "Lobby created successfully!";

	return crow::response(200, data);
}

crow::response LobbyRoutes::JoinLobby(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	const std::string lobbyId = body["lobbyId"].s();
	const std::string username = body["username"].s();

	auto userOpt = m_db.GetUserByName(username);
	if (!userOpt) return crow::response(404, "User not found");
	unsigned playerId = userOpt.value().GetID();

	auto lobby = m_manager.GetLobby(lobbyId);	
	if (!lobby) return crow::response(404, "Lobby not found");

	auto playerPtr = std::make_shared<Player>(playerId, username);

	if (!lobby->AddPlayer(playerId, playerPtr)) return crow::response
	(409, "Lobby full or player already exists");

	crow::json::wvalue data;
	data["status"] = "OK";
	data["playerId"] = playerId;
	data["lobbyId"] = lobbyId;

	return crow::response(data);
}

crow::response LobbyRoutes::LeaveLobby(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	const std::string lobbyId = body["lobbyId"].s();
	const unsigned playerId = body["playerId"].i();

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	auto result = lobby->RemovePlayer(playerId);
	if (result == RemoveResult::NotFound)
	{
		return crow::response(404, "Player not found");
	}
	if (result == RemoveResult::HostRemoved)
	{
		m_manager.RemoveLobby(lobbyId);
		crow::json::wvalue data;
		data["status"] = "LOBBY_DELETED";
		data["message"] = "Host left. Lobby closed.";
		return crow::response(200, data);
	}

	crow::json::wvalue data;
	data["status"] = "PLAYER_LEFT";
	data["message"] = "Player left lobby.";
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetLobbyState(const std::string& lobbyId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	crow::json::wvalue data;

	if (lobby->GetPlayers().size() < Lobby::K_MIN_PLAYERS)
	{
		data["status"] = "NOT_ENOUGH_PLAYERS";
		data["message"] = "Waiting for players.";
		return crow::response(200, data);
	}

	if (lobby->IsFull())
	{
		data["status"] = "FULL_LOBBY";
		data["message"] = "Lobby is full.";
		return crow::response(200, data);
	}

	if (lobby->HasGameStarted())
	{
		data["status"] = "GAME_STARTED";
		data["message"] = "Match is ongoing.";
		return crow::response(200, data);
	}
	data["status"] = "WAITING_FOR_PLAYERS";
	data["message"] = "Match hasn't started.";
	return crow::response(data);
}

crow::response LobbyRoutes::StartMatch(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	const std::string lobbyId = body["lobbyId"].s();
	const unsigned playerId = body["playerId"].i();

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	if (!lobby->IsPlayerHost(playerId)) return crow::response
	(409, "Player is not host.");

	crow::json::wvalue data;
	
	if (lobby->GetPlayers().empty()) return crow::response(409, "Lobby empty.");

	if (lobby->GetPlayers().size() < Lobby::K_MIN_PLAYERS)
	{
		data["status"] = "NOT_ENOUGH_PLAYERS";
		data["message"] = "Waiting for players.";
		return crow::response(200, data);
	}

	if (lobby->HasGameStarted())
	{
		data["status"] = "ALREADY_STARTED";
		data["message"] = "Game already in progress.";
		return crow::response(200, data);
	}
	
	data["status"] = "GAME_STARTED";
	data["message"] = "Game has started.";

	return crow::response(200, data);
}