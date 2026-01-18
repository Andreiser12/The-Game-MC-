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
	lobbyId.resize(K_LOBBY_ID_LENGTH);

	for (auto& c : lobbyId) c = charset[dist(gen)];
	return lobbyId;
}

LobbyRoutes::LobbyRoutes(LobbyManager& manager, DatabaseManager& db) :
	m_manager{ manager },
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

	CROW_ROUTE(app, "/lobby/start").methods("POST"_method)
		([this](const crow::request& req) {
		return this->StartMatch(req);
			});

	CROW_ROUTE(app, "/lobby/<string>/players").methods("GET"_method)
		([this](const std::string& lobbyId) {
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

	CROW_ROUTE(app, "/lobbies/<string>/players/<uint>").methods("DELETE"_method)
		([this](const std::string& lobbyId, unsigned playerId)
			{
				return this->LeaveMatch(lobbyId, playerId);
			});

	CROW_ROUTE(app, "/lobby/<string>/players/states").methods("GET"_method)
		([this](const std::string& lobbyId)
			{
				return this->GetPlayerStates(lobbyId);
			});

	CROW_ROUTE(app, "/lobby/lost").methods("POST"_method)
		([this](const crow::request& req) {
		return this->ReportGameLoss(req);
			});

	CROW_ROUTE(app, "/lobby/<string>/players/<uint>/can_play").methods("GET"_method)
		([this](const crow::request& req, const std::string& lobbyId, unsigned playerId) {
		return this->GetCanPlayMore(req, lobbyId, playerId);
			});

	CROW_ROUTE(app, "/lobby/<string>/info").methods("GET"_method)
		([this](const std::string& lobbyId) {
		return this->GetLobbyInfo(lobbyId);
			});

	CROW_ROUTE(app, "/lobby/<string>/players/<uint>/place-cards-AI").methods("POST"_method)
		([this](const crow::request& req, const std::string& lobbyId, unsigned playerId) {
		return this->GetAICards(req, lobbyId, playerId);
			});
 }

crow::response LobbyRoutes::CreateLobby(const crow::request& request)
{
	auto json = crow::json::load(request.body);
	if (!json) return crow::response(400, "Invalid JSON");

	const std::string lobbyId{ GenerateLobbyId() };
	auto lobby = m_manager.CreateLobby(lobbyId,
		static_cast<unsigned>(json["playerId"].i()), json["username"].s());
	if (!lobby) return crow::response(500, "Failed to create lobby");

	crow::json::wvalue data;
	data["lobbyId"] = lobbyId;
	data["status"] = "Lobby created successfully!";
	return crow::response(200, data);
}

crow::response LobbyRoutes::JoinLobby(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body
		|| !body.has("playerId")
		|| !body.has("lobbyId")) return crow::response(400, "Invalid JSON");

	const std::string lobbyId{ body["lobbyId"].s() };
	const std::string username{ body["username"].s() };
	unsigned playerId = static_cast<unsigned>(body["playerId"].i());

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	if (lobby->IsGameActive())
	{
		crow::json::wvalue errorData;
		errorData["error"] = "game_started";
		return crow::response(409, errorData);
	}

	if (!lobby->AddPlayer(playerId, username))
	{
		crow::json::wvalue errorData;
		errorData["error"] = "lobby_full";
		return crow::response(409, errorData);
	}

	crow::json::wvalue data;
	data["playerId"] = playerId;
	return crow::response(200, data);
}

crow::response LobbyRoutes::LeaveLobby(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	const std::string lobbyId{ body["lobbyId"].s() };
	const unsigned playerId{ static_cast<unsigned>(body["playerId"].i()) };

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	auto result = lobby->RemovePlayer(playerId);
	if (result == RemoveResult::NotFound) return crow::response(404, "Player not found");

	crow::json::wvalue data;
	if (result == RemoveResult::HostRemoved)
	{
		m_manager.RemoveLobby(lobbyId);
		data["status"] = "LOBBY_DELETED";
		data["message"] = "Host left. Lobby closed.";
		return crow::response(200, data);
	}

	data["status"] = "PLAYER_LEFT";
	data["message"] = "Player left lobby.";
	return crow::response(200, data);
}

crow::response LobbyRoutes::LeaveMatch(const std::string& lobbyId, unsigned playerId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	crow::json::wvalue data;
	const auto result = lobby->RemoveMatchPlayer(playerId);
	switch (result)
	{
	case RemoveResult::Removed:
	{
		auto gameResultFound = lobby->GetGameResult();
		if (gameResultFound && *gameResultFound != GameResult::Ongoing && !lobby->StatsAlreadyUpdated())
		{
			const double hours = lobby->GetMatchDurationHours();
			for (const auto& [id, info] : lobby->GetPlayers())
			{
				m_db.UpdateStats(id, true, false, hours);
			}
			lobby->MarkStatsUpdated();
		}
		data["PLAYER_LEFT"] = "SUCCESS";
		return crow::response(200, data);
	}
	case RemoveResult::NotFound:
		return crow::response(404);
	}

	return crow::response(500);
}

crow::response LobbyRoutes::StartMatch(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	const std::string lobbyId{ body["lobbyId"].s() };
	const unsigned playerId{ static_cast<unsigned>(body["playerId"].i()) };

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	if (lobby->StartGame(playerId))
	{
		crow::json::wvalue data;
		data["status"] = "GAME_STARTED";
		return crow::response(200, data);
	}
	return crow::response(403, "Failed to start: Not host or not enough players");
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
		playersList.emplace_back(player.username);
	}
	data["players"] = std::move(playersList);

	return crow::response(200, data);
}

crow::response LobbyRoutes::SendChatMessage(const crow::request& req)
{
	auto json = crow::json::load(req.body);
	if (!json) return crow::response(400, "Invalid JSON");

	const std::string lobbyId{ json["lobbyId"].s() };
	const std::string username{ json["username"].s() };
	const unsigned playerId{ static_cast<unsigned>(json["playerId"].i()) };
	const std::string message{ json["message"].s() };
	const bool isSystem{ json["isSystem"].b() };

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	lobby->AddChatMessage(username, message, isSystem);

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
		messageJson["isSystem"] = newMsg.isSystem;
		messageList.push_back(std::move(messageJson));
	}
	data["messages"] = std::move(messageList);

	return crow::response(200, data);
}

crow::response LobbyRoutes::PlayerPing(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	const std::string lobbyId{ body["lobbyId"].s() };
	const unsigned playerId{ static_cast<unsigned>(body["playerId"].i()) };

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
		cardList.emplace_back(cardId);
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
	for (size_t i = 0; i < decksState.value().size(); ++i)
	{
		crow::json::wvalue deck;
		deck["deckId"] = decksState.value()[i].deckId;
		deck["topCardId"] = decksState.value()[i].topCardId;
		deck["size"] = decksState.value()[i].size;
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

	if (!drawPileCount
		|| !decksState) return crow::response(409, "Game not started");

	crow::json::wvalue data;
	data["drawPileCount"] = static_cast<size_t>(drawPileCount.value());
	data["piles"] = crow::json::wvalue::list();
	for (size_t i = 0; i < decksState.value().size(); ++i)
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
	data["playerId"] = stateOpt.value().playerId;
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

	auto gameResultFound = lobby->GetGameResult();
	if (gameResultFound && *gameResultFound != GameResult::Ongoing && !lobby->StatsAlreadyUpdated())
	{
		const double hours = lobby->GetMatchDurationHours();
		const bool isWin = (*gameResultFound == GameResult::Won);

		for (const auto& [id, info] : lobby->GetPlayers())
		{
			m_db.UpdateStats(id, true, isWin, hours);
		}
		lobby->MarkStatsUpdated();
	}

	crow::json::wvalue data;
	data["cards"] = crow::json::wvalue::list();
	for (size_t i = 0; i < drawnCards.value().size(); ++i)
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
	data["currentUsername"] = turnOpt.value().currentUsername;
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
	case GameResult::NotEnoughPlayers:
		data["result"] = "NOT_ENOUGH_PLAYERS";
		break;
	default:
		return crow::response(500);
	}
	return crow::response(200, data);
}

crow::response LobbyRoutes::ReportGameLoss(const crow::request& request)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	const std::string lobbyId{ body["lobbyId"].s() };

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	lobby->SetGameLost();

	crow::json::wvalue data;
	data["status"] = "OK";
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetValidDecks(const crow::request& request, const std::string& lobbyId)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	const uint8_t cardId{ static_cast<uint8_t>(body["cardId"].i()) };
	auto validDecksOpt{ lobby->GetValidDecks(cardId) };
	if (!validDecksOpt) return crow::response(409, "Cannot get valid decks");

	const auto& validDecks = validDecksOpt.value();
	crow::json::wvalue data;
	data["decks"] = crow::json::wvalue::list();
	for (size_t i = 0; i < validDecks.size(); ++i)
	{
		data["decks"][i] = validDecks[i];
	}
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetPlayerStates(const std::string& lobbyId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	const auto& statesOpt = lobby->GetPlayerStates();
	if (!statesOpt) return crow::response(409);
	const auto& states = statesOpt.value();

	crow::json::wvalue data;
	data["USERNAMES"] = crow::json::wvalue::list(states.size());
	data["IS_IN_MATCH"] = crow::json::wvalue::list(states.size());
	data["PLAYERIDS"] = crow::json::wvalue::list(states.size());
	for (size_t i = 0; i < states.size(); ++i)
	{
		data["PLAYERIDS"][i] = states[i].playerId;
		data["USERNAMES"][i] = states[i].username;
		data["IS_IN_MATCH"][i] = states[i].isInMatch;
	}
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetCanPlayMore(const crow::request& request,
	const std::string& lobbyId, unsigned playerId)
{
	auto body = crow::json::load(request.body);
	if (!body) return crow::response(400, "Invalid JSON");

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	const uint8_t remainingCards{ static_cast<uint8_t>(body["remaining_cards"].i()) };
	const auto canPlayMore = lobby->GetCanPlayMore(playerId, remainingCards);
	if (!canPlayMore) return crow::response(409, "Cannot get result");

	crow::json::wvalue data;
	data["can_play"] = canPlayMore.value();
	return crow::response(200, data);
}

crow::response LobbyRoutes::GetLobbyInfo(const std::string& lobbyId)
{
	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby)
	{
		crow::json::wvalue errorData;
		errorData["error"] = "Lobby not found";
		return crow::response(404, errorData);
	}

	crow::json::wvalue data;
	data["status"] = lobby->IsGameActive() ? "in_progress" : "waiting";
	data["current_players"] = static_cast<int>(lobby->GetPlayerCount());
	data["max_players"] = 5;
	data["is_game_started"] = lobby->IsGameActive();
	data["host_id"] = lobby->GetHostId();

	return crow::response(200, data);
}

crow::response LobbyRoutes::GetAICards(const crow::request& request, const std::string& lobbyId, unsigned playerId)
{
	auto body = crow::json::load(request.body);
	if (!body || !body.has("cards_required")) return crow::response(400, "Invalid JSON");

	auto lobby = m_manager.GetLobby(lobbyId);
	if (!lobby) return crow::response(404, "Lobby not found");

	const uint8_t cardsRequired{ static_cast<uint8_t>(body["cards_required"].i()) };
	auto cardsOpt = lobby->GetAICards(playerId, cardsRequired);
	if (!cardsOpt) return crow::response(409, "Couldn't get cards");

	crow::json::wvalue data;
	const auto& cards = cardsOpt.value();
	data["card_id"] = crow::json::wvalue::list(cards.size());
	data["deck_id"] = crow::json::wvalue::list(cards.size());
	for (size_t i = 0; i<cards.size(); ++i)
	{
		data["card_id"][i] = static_cast<int>(cards[i].first);
		data["deck_id"][i] = static_cast<int>(cards[i].second);
	}
	return crow::response(200, data);
}
