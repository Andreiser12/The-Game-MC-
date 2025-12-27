import LobbyRoutes;
import SessionManager;
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
	for (size_t i = 0; i < K_LOBBY_ID_LENGTH; i++)
	{
		lobbyId += charset[dist(gen)];
	}
	return lobbyId;
}

LobbyRoutes::LobbyRoutes(LobbyManager& manager, DatabaseManager& db)
	: m_manager{ manager },
	m_db{ db } {
}

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

	CROW_ROUTE(app, "/lobby/<string>/players").methods("GET"_method)
		([&](const std::string& lobbyId) {
		return this->FetchLobbyPlayers(lobbyId);
			});

	CROW_ROUTE(app, "/lobby/chat/send").methods("POST"_method)
		([this](const crow::request& req) {
		return this->SendChatMessage(req);
			});

	CROW_ROUTE(app, "/lobby/<string>/chat/messages").methods("GET"_method)
		([this](const crow::request& req, const std::string& lobbyId) {
		return this->GetNewMessages(req, lobbyId);
			});

	CROW_ROUTE(app, "/lobby/ping").methods("POST"_method)
		([this](const crow::request& req) {
		return this->PlayerPing(req);
			});

	CROW_ROUTE(app, "/lobby/<string>/players/<uint>/cards").methods("GET"_method)
		([this](const std::string& lobbyId, unsigned playerId)
			{
				return this->GetPlayerCards(lobbyId, playerId);
			});

	CROW_ROUTE(app, "/lobby/<string>/players/<uint>/cards").methods("POST"_method)
		([this](const crow::request& req, const std::string& lobbyId, unsigned playerId)
			{
				return this->PlayCard(req, lobbyId, playerId);
			});

	CROW_ROUTE(app, "/lobby/<string>/decks").methods("GET"_method)
		([this](const std::string& lobbyId)
			{
				return this->GetDecksState(lobbyId);
			});

	CROW_ROUTE(app, "/lobby/<string>/game/state").methods("GET"_method)
		([this](const std::string& lobbyId)
			{
				return this->GetGameState(lobbyId);
			});
	
	CROW_ROUTE(app, "/lobby/<string>/game/result").methods("GET"_method)
		([this](const std::string& lobbyId)
			{
				return this->GetGameResult(lobbyId);
			});

	CROW_ROUTE(app, "/lobby/<string>/players/<uint>/endturn").methods("GET"_method)
		([this](const std::string& lobbyId, unsigned playerId)
			{
				return this->GetEndTurnState(lobbyId, playerId);
			});

	CROW_ROUTE(app, "/lobby/<string>/players/<uint>/endturn").methods("POST"_method)
		([this](const std::string& lobbyId, unsigned playerId)
			{
				return this->EndTurn(lobbyId, playerId);
			});

	CROW_ROUTE(app, "/lobby/<string>/players/<uint>/turn").methods("GET"_method)
		([this](const std::string& lobbyId, unsigned playerId)
			{
				return this->GetTurnState(lobbyId, playerId);
			});

	CROW_ROUTE(app, "/lobby/<string>/decks/valid").methods("GET"_method)
		([this](const crow::request& req, const std::string& lobbyId)
			{
				return this->GetValidDecks(req, lobbyId);
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

	if (!lobby->AddPlayer(playerId, username)) return crow::response
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
	if (result == RemoveResult::NotFound) return crow::response(404, "Player not found");

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
	data["hostId"] = lobby->GetHostId();

	if (lobby->HasGameStarted())
	{
		data["status"] = "GAME_STARTED";
		data["message"] = "Match is ongoing.";
		return crow::response(200, data);
	}

	if (lobby->GetPlayers().size() < Lobby::kMinPlayers)
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

	data["status"] = "WAITING_FOR_PLAYERS";
	data["message"] = "Match hasn't started.";
	return crow::response(200, data);
}

crow::response LobbyRoutes::StartMatch(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	const std::string lobbyId = body["lobbyId"].s();
	const unsigned playerId = body["playerId"].i();

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	if (!lobby->IsPlayerHost(playerId)) return crow::response(409, "Player is not host.");

	if (lobby->GetPlayers().empty()) return crow::response(409, "Lobby empty.");

	crow::json::wvalue data;
	if (lobby->GetPlayers().size() < Lobby::kMinPlayers)
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

	if (!lobby->StartGame(playerId)) return crow::response(409, "Start rejected.");

	data["status"] = "GAME_STARTED";
	data["message"] = "Game has started.";
	return crow::response(200, data);
}

crow::response LobbyRoutes::FetchLobbyPlayers(const std::string& lobbyId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	crow::json::wvalue data;
	crow::json::wvalue::list playersList;

	const auto& players = lobby->GetPlayers();
	for (const auto& [playerId, player] : players)
	{
		playersList.push_back(player->GetUsername());
	}
	data["players"] = std::move(playersList);

	return crow::response(200, data);
}
crow::response LobbyRoutes::SendChatMessage(const crow::request& req)
{
	auto json = crow::json::load(req.body);
	if (!json) return crow::response(400, "Invalid JSON");

	const std::string lobbyId = json["lobbyId"].s();
	const std::string username = json["username"].s();
	const unsigned playerId = json["playerId"].i();
	const std::string message = json["message"].s();

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	lobby->AddChatMessage(username, message);

	crow::json::wvalue data;
	data["status"] = "OK";
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetNewMessages(const crow::request& request, const std::string& lobbyId)
{
	unsigned afterId{ 0u };
	if (auto afterString = request.url_params.get("after"))
	{
		try {
			afterId = std::stoul(afterString);
		}
		catch (...)
		{
			return crow::response(400, "Invalid 'after' parameter");
		}
	}

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	const auto& newMessages = lobby->GetMessagesAfter(afterId);
	if (!newMessages || newMessages->empty()) return crow::response(204, "No content");

	crow::json::wvalue data;
	crow::json::wvalue::list messageList;
	messageList.reserve(newMessages->size());

	for (const auto& newMsg : newMessages.value())
	{
		crow::json::wvalue messageJson;
		messageJson["id"] = newMsg.id;
		messageJson["username"] = newMsg.username;
		messageJson["message"] = newMsg.message;
		messageJson["timestamp"] = std::chrono::system_clock::now().time_since_epoch().count();
		messageList.push_back(std::move(messageJson));
	}
	data["messages"] = std::move(messageList);

	return crow::response(200, data);
}

crow::response LobbyRoutes::PlayerPing(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	const std::string lobbyId = body["lobbyId"].s();
	const unsigned playerId = body["playerId"].i();

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	lobby->UpdatePlayerPing(playerId);
	auto& sessionManager = SessionManager::Instance();
	sessionManager.UpdateActivity(playerId);

	crow::json::wvalue data;
	data["status"] = "OK";
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetPlayerCards(const std::string& lobbyId, unsigned playerId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	auto cardsOpt = lobby->GetPlayerCards(playerId);
	if (!cardsOpt) return crow::response(409, "Game state does not allow getting cards");

	crow::json::wvalue data;
	crow::json::wvalue::list cardList;
	for (uint8_t cardId : cardsOpt.value())
	{
		cardList.push_back(cardId);
	}
	data["cards"] = std::move(cardList);
	return crow::response(200, data);
}

crow::response LobbyRoutes::PlayCard(const crow::request& request, const std::string& lobbyId, unsigned playerId)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	if (!lobby->PlayCard(playerId, static_cast<uint8_t>(body["cardId"].i()),
		static_cast<uint8_t>(body["deckId"].i())))
	{
		return crow::response(409, "Card cannot be placed");
	}
	crow::json::wvalue data;
	data["status"] = "OK";
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetDecksState(const std::string& lobbyId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	auto decksState = lobby->GetDecksState();
	if (!decksState) return crow::response(409, "Game not started");

	crow::json::wvalue data;
	data["decks"] = crow::json::wvalue::list();
	for (size_t i = 0; i < decksState.value().size(); i++)
	{
		crow::json::wvalue deck;
		deck["deckId"] = decksState.value()[i].deckId;
		deck["topCardId"] = decksState.value()[i].topCardId;
		data["decks"][i] = std::move(deck);
	}
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetGameState(const std::string& lobbyId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	auto drawPileCount = lobby->GetDrawPileCount();
	auto decksState = lobby->GetDecksState();

	if (!drawPileCount.has_value() || !decksState.has_value()) return crow::response(409, "Game not started");

	crow::json::wvalue data;
	data["drawPileCount"] = static_cast<unsigned>(drawPileCount.value());
	data["piles"] = crow::json::wvalue::list();
	for (size_t i = 0; i < decksState.value().size(); i++)
	{
		crow::json::wvalue deck;
		deck["count"] = 0;
		deck["top"] = decksState.value()[i].topCardId;
		data["piles"][i] = std::move(deck);
	}
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetEndTurnState(const std::string& lobbyId, unsigned playerId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	auto stateOpt = lobby->GetEndTurnState(playerId);
	if (!stateOpt) return crow::response(409, "Game not started or invalid player");

	crow::json::wvalue data;
	data["canEndTurn"] = stateOpt.value().canEndTurn;
	data["cardsPlaced"] = stateOpt.value().cardsPlaced;
	data["minRequiredCards"] = stateOpt.value().minRequiredCards;
	return crow::response(200, data);
}

crow::response LobbyRoutes::EndTurn(const std::string& lobbyId, unsigned playerId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	auto drawnCards = lobby->EndTurn(playerId);
	if (!drawnCards) return crow::response(409, "End turn not allowed");

	crow::json::wvalue data;
	data["cards"] = crow::json::wvalue::list();
	for (size_t i = 0; i<drawnCards.value().size(); i++)
	{
		data["cards"][i] = drawnCards.value()[i];
	}
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetTurnState(const std::string& lobbyId, unsigned playerId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	auto turnOpt = lobby->GetTurnState(playerId);
	if (!turnOpt) return crow::response(409, "Cannot get player turn");

	crow::json::wvalue data;
	data["isMyTurn"] = turnOpt.value().isMyTurn;
	data["currentPlayerId"] = turnOpt.value().currentPlayerId;
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetGameResult(const std::string& lobbyId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	auto gameResult = lobby->GetGameResult();
	if (!gameResult) return crow::response(409, "Game has not started");

	crow::json::wvalue data;
	switch (gameResult.value())
	{
		case GameResult::Won:
			data["result"] = "won";
			break;
		case GameResult::Lost:
			data["result"] = "lost";
			break;
		case GameResult::Ongoing:
			data["result"] = "ongoing";
			break;
	}
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetValidDecks(const crow::request& request, const std::string& lobbyId)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	const uint8_t cardId = static_cast<uint8_t>(body["cardId"].i());
	auto validDecksOpt{ lobby->GetValidDecks(cardId) };
	if (!validDecksOpt) return crow::response(409, "Cannot get valid decks");

	crow::json::wvalue data;
	data["decks"] = crow::json::wvalue::list();
	for (size_t i=0; i<validDecksOpt.value().size(); i++)
	{
		data["decks"][i] = validDecksOpt.value()[i];
	}
	return crow::response(200, data);
}
