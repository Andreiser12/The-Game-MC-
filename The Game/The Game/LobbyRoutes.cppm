module;

#include "crow.h"
import <cstdint>;

export module LobbyRoutes;

import LobbyManager;
import Lobby;
import DatabaseManager;
import DatabaseStructures;
import SessionManager;

export class LobbyRoutes
{
public:
	LobbyRoutes(LobbyManager& manager, DatabaseManager& db);
	void Register(crow::SimpleApp& app);

private:
	crow::response CreateLobby(const crow::request& request);
	crow::response JoinLobby(const crow::request& request);
	crow::response LeaveLobby(const crow::request& request);
	crow::response LeaveMatch(const std::string& lobbyId, unsigned playerId);
	crow::response StartMatch(const crow::request& request);
	crow::response FetchLobbyPlayers(const std::string& lobbyId);
	crow::response SendChatMessage(const crow::request& request);
	crow::response GetNewMessages(const crow::request& request, const std::string& lobbyId);
	crow::response PlayerPing(const crow::request& request);
	crow::response GetPlayerCards(const std::string& lobbyId, unsigned playerId);
	crow::response PlayCard(const crow::request& request, const std::string& lobbyId,
		unsigned playerId);
	crow::response GetDecksState(const std::string& lobbyId);
	crow::response GetGameState(const std::string& lobbyId);
	crow::response GetEndTurnState(const std::string& lobbyId, unsigned playerId);
	crow::response EndTurn(const std::string& lobbyId, unsigned playerId);
	crow::response GetTurnState(const std::string& lobbyId, unsigned playerId);
	crow::response GetGameResult(const std::string& lobbyId);
	crow::response ReportGameLoss(const crow::request& request);
	crow::response GetValidDecks(const crow::request& request, const std::string& lobbyId);
	crow::response GetPlayerStates(const std::string& lobbyId);
	crow::response GetCanPlayMore(const crow::request& request,
		const std::string& lobbyId, unsigned playerId);
	crow::response GetLobbyInfo(const std::string& lobbyId);
	crow::response GetAICards(const crow::request& request, const std::string& lobbyId,
		unsigned playerId);

private:
	LobbyManager& m_manager;
	DatabaseManager& m_db;
};