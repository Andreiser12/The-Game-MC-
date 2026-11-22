module;
#define NOMINMAX
#include <algorithm> 
#include <cmath>     
#include <cpr/cpr.h>
#include "crow.h"

module ApiClient;

import UserProfile;

static inline cpr::Timeout kTimeout{ 400 };

ApiClient::ApiClient(const std::string& serverURL) : m_serverURL{ serverURL } {}

LoginResult ApiClient::Login(const std::string& username)
{
	const std::string url{ m_serverURL + "/login" };
	const std::string jsonBody{ "{\"username\":\"" + username + "\"}" };

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);

	if (response.error || response.status_code != 200) return LoginResult::NetworkError;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("status")) return LoginResult::NetworkError;

	if (json["status"].s() != "ok")
	{
		const std::string message = json["message"].s();
		if (message == "You're already logged in!") return LoginResult::AlreadyLoggedIn;
		return LoginResult::UserNotFound;
	}

	std::lock_guard<std::mutex> lock(m_mutex);
	m_username = username;
	m_playerId = static_cast<unsigned>(json["id"].i());
	return LoginResult::Success;
}

bool ApiClient::Logout()
{
	unsigned playerIdCopy;
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_playerId == 0u) return false;
		playerIdCopy = m_playerId;
	}

	const std::string url{ m_serverURL + "/logout" };
	std::string jsonBody{ "{\"playerId\":" + std::to_string(playerIdCopy) + "}" };

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);

	if (response.error || response.status_code != 200) return false;

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_playerId = 0;
		m_username.clear();
		m_lobbyId.clear();
	}
	return true;
}

bool ApiClient::IsLoggedIn()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_playerId != 0u;
}

bool ApiClient::DeleteAccount()
{
	unsigned playerId;
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_playerId == 0u) return false;
		playerId = m_playerId;
	}
	const std::string url = m_serverURL + "/users/" + std::to_string(playerId);
	cpr::Response response = cpr::Delete(
		cpr::Url{ url },
		kTimeout
	);
	if (response.error || response.status_code != 200)
		return false;

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_playerId = 0;
		m_username.clear();
		m_lobbyId.clear();
	}
	return true;
}

std::optional<std::string> ApiClient::CreateLobby(const std::string& username) const
{
	const std::string url{ m_serverURL + "/lobby/create" };
	std::string jsonBody{ "{\"username\":\"" + username +
		"\",\"playerId\":\"" + std::to_string(m_playerId) + "\"}" };

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("lobbyId")) return std::nullopt;
	return json["lobbyId"].s();
}

JoinLobbyResult ApiClient::JoinLobbyValidated(const std::string& lobbyId, const std::string& username)
{
	auto infoOpt = GetLobbyInfo(lobbyId);
	if (!infoOpt) return JoinLobbyResult::NetworkError;

	const auto& info = infoOpt.value();

	if (info.has("is_game_started") && info["is_game_started"].b())
		return JoinLobbyResult::GameAlreadyStarted;

	if (info.has("current_players") && info.has("max_players") &&
		info["current_players"].i() >= info["max_players"].i())
		return JoinLobbyResult::LobbyFull;

	const std::string url{ m_serverURL + "/lobby/join" };
	std::string jsonBody{ "{"
		"\"lobbyId\":\"" + lobbyId + "\","
		"\"username\":\"" + username + "\","
		"\"playerId\":" + std::to_string(m_playerId) +
		"}" };

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);
	if (response.error) return JoinLobbyResult::NetworkError;

	auto json = crow::json::load(response.text);
	if (response.status_code == 200)
	{
		if (!json || !json.has("playerId")) return JoinLobbyResult::NetworkError;

		std::lock_guard<std::mutex> lock(m_mutex);
		m_playerId = static_cast<unsigned>(json["playerId"].i());
		m_lobbyId = lobbyId;
		return JoinLobbyResult::Success;
	}

	if (response.status_code == 409 && json && json.has("error"))
	{
		if (json["error"].s() == "lobby_full") return JoinLobbyResult::LobbyFull;
		if (json["error"].s() == "game_started") return JoinLobbyResult::GameAlreadyStarted;
	}
	return JoinLobbyResult::NetworkError;
}

bool ApiClient::LeaveLobby(const std::string& lobbyId, unsigned playerId)
{
	const std::string url = m_serverURL + "/lobby/leave";

	std::string jsonBody =
		"{\"lobbyId\":\"" + lobbyId +
		"\",\"playerId\":" + std::to_string(playerId) +
		"}";
	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);

	if (response.error || response.status_code != 200) return false;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("status")) return false;

	if (json["status"].s() == "PLAYER_LEFT")
		m_event.store(ClientEvent::PlayerLeft);
	else if (json["status"].s() == "LOBBY_DELETED")
		m_event.store(ClientEvent::HostLeftLobby);
	else return false;

	return true;
}

std::optional<LeaveMatchResult> ApiClient::LeaveMatch()
{
	const std::string url = m_serverURL + "/lobbies/" + m_lobbyId
		+ "/players/" + std::to_string(m_playerId);

	cpr::Response response = cpr::Delete(
		cpr::Url{ url },
		cpr::Timeout(kTimeout)
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("PLAYER_LEFT")) return std::nullopt;

	if (json["PLAYER_LEFT"].s() == "SUCCESS")
	{
		auto playerStates = GetPlayerStates();
		{
			std::lock_guard<std::mutex> lk(m_cacheMutex);
			m_cachedPlayerStates = playerStates;
		}
		return LeaveMatchResult::Success;
	}
	return LeaveMatchResult::NotInLobby;
}

std::optional<std::vector<PlayerState>> ApiClient::GetPlayerStates() const
{
	const std::string url{ m_serverURL + "/lobby/" + m_lobbyId
		+ "/players/states" };
	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		kTimeout
	);
	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("PLAYERIDS") || !json.has("USERNAMES") || !json.has("IS_IN_MATCH")) return std::nullopt;

	const auto& ids = json["PLAYERIDS"];
	const auto& usernames = json["USERNAMES"];
	const auto& isInMatch = json["IS_IN_MATCH"];

	if (ids.size() != usernames.size() || usernames.size() != isInMatch.size()) return std::nullopt;

	std::vector<PlayerState> playerStates;
	playerStates.reserve(usernames.size());
	for (size_t i = 0; i < usernames.size(); ++i)
	{
		playerStates.emplace_back(
			PlayerState{
				static_cast<unsigned>(ids[i].i()),
				usernames[i].s(),
				isInMatch[i].b()
			}
		);
	}
	return playerStates;
}

bool ApiClient::StartMatch(const std::string& lobbyId, unsigned playerId) const
{
	const std::string url{ m_serverURL + "/lobby/start" };
	const std::string jsonBody{
		"{\"lobbyId\":\"" + lobbyId +
		"\",\"playerId\":" + std::to_string(playerId) +
		"}" };

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);

	if (response.error || response.status_code != 200) return false;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("status")) return false;

	return json["status"].s() == "GAME_STARTED";
}

bool ApiClient::SendChatMessage(const std::string& message, bool isSystem) const
{
	const std::string url{ m_serverURL + "/lobby/chat/send" };
	crow::json::wvalue jsonBody;
	jsonBody["lobbyId"] = m_lobbyId;
	jsonBody["username"] = m_username;
	jsonBody["playerId"] = m_playerId;
	jsonBody["isSystem"] = isSystem;
	jsonBody["message"] = message;

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody.dump() },
		kTimeout);

	if (response.error || response.status_code != 200) return false;

	return true;
}

std::optional<std::vector<ChatMessage>> ApiClient::GetNewMessages(unsigned afterId)
{
	if (m_lobbyId.empty()) return std::nullopt;

	const std::string url{ m_serverURL + "/lobby/" + m_lobbyId
		+ "/chat/messages?after=" + std::to_string(afterId) };

	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("messages")) return std::nullopt;

	std::vector<ChatMessage> result;
	for (const auto& message : json["messages"])
	{
		result.emplace_back(
			static_cast<unsigned>(message["id"].i()),
			message["username"].s(),
			message["message"].s(),
			message["isSystem"].b());
	}
	return result;
}

bool ApiClient::PlayCard(uint8_t cardId, uint8_t deckId) const
{
	const std::string url = m_serverURL + "/lobby/" + m_lobbyId
		+ "/players/" + std::to_string(m_playerId) + "/cards";
	const std::string jsonBody =
		"{"
		"\"cardId\":" + std::to_string(cardId) + ","
		"\"deckId\":" + std::to_string(deckId) +
		"}";
	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);
	if (response.error || response.status_code != 200) return false;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("status")) return false;

	return json["status"].s() == "OK";
}

std::optional<std::vector<DeckState>> ApiClient::GetDecksState() const
{
	const std::string url{ m_serverURL + "/lobby/" + m_lobbyId +
		"/decks" };
	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("decks")) return std::nullopt;

	std::vector<DeckState> deckStates;
	for (const auto& deckJson : json["decks"])
	{
		DeckState deck;
		deck.deckId = static_cast<uint8_t>(deckJson["deckId"].i());
		deck.topCardId = static_cast<uint8_t>(deckJson["topCardId"].i());
		deck.size = static_cast<uint8_t>(deckJson["size"].i());
		deckStates.emplace_back(deck);
	}
	return deckStates;
}

std::optional<EndTurnState> ApiClient::GetEndTurnState() const
{
	if (m_lobbyId.empty() || m_playerId == 0u) return std::nullopt;

	const std::string url{ m_serverURL + "/lobby/" + m_lobbyId +
		"/players/" + std::to_string(m_playerId) + "/endturn" };
	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		kTimeout
	);
	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json
		|| !json.has("playerId")
		|| !json.has("canEndTurn")
		|| !json.has("cardsPlaced")
		|| !json.has("minRequiredCards")) return std::nullopt;

	EndTurnState state;
	state.playerId = static_cast<unsigned>(json["playerId"].i());
	state.canEndTurn = json["canEndTurn"].b();
	state.cardsPlaced = static_cast<uint8_t>(json["cardsPlaced"].i());
	state.minRequiredCards = static_cast<uint8_t>(json["minRequiredCards"].i());
	return state;
}

bool ApiClient::SendPing(const std::string& lobbyId, unsigned playerId) const
{
	const std::string url = m_serverURL + "/lobby/ping";
	std::string jsonBody = "{\"lobbyId\":\"" + lobbyId + "\",\"playerId\":" + std::to_string(playerId) + "}";

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);

	if (response.error || response.status_code != 200) return false;

	auto json = crow::json::load(response.text);
	return json && json["status"].s() == "OK";
}

ClientEvent ApiClient::GetEvent() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_event.load();
}

void ApiClient::ClearEvent()
{
	m_event.store(ClientEvent::None);
}

void ApiClient::SetUsername(const std::string& username)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_username = username;
}

void ApiClient::SetPlayerId(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_playerId = playerId;
}

void ApiClient::SetLobbyId(const std::string& lobbyId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_lobbyId = lobbyId;
}

void ApiClient::SetGameActive(bool active)
{
	m_isGameActive.store(active);
}

bool ApiClient::ReportGameLoss()
{
	std::string lobbyId;
	unsigned playerId;
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		lobbyId = m_lobbyId;
		playerId = m_playerId;
	}

	if (lobbyId.empty() || playerId == 0u) return false;

	const std::string url = m_serverURL + "/lobby/lost";

	const std::string jsonBody =
		"{"
		"\"lobbyId\":\"" + lobbyId + "\","
		"\"playerId\":" + std::to_string(playerId) +
		"}";

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);

	if (response.error || response.status_code != 200) return false;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("status")) return false;

	return json["status"].s() == "OK";
}

std::string ApiClient::GetUsername() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_username;
}

unsigned ApiClient::GetPlayerId() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_playerId;
}

std::string ApiClient::GetLobbyId() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_lobbyId;
}

std::optional<UserProfile> ApiClient::GetUserProfile(unsigned playerId) const
{
	const std::string url = m_serverURL + "/users/" + std::to_string(playerId) + "/profile";
	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("username")
		|| !json.has("gamesPlayed")
		|| !json.has("gamesWon")
		|| !json.has("hoursPlayed")) return std::nullopt;

	UserProfile profile;
	profile.username = json["username"].s();
	profile.gamesPlayed = json["gamesPlayed"].u();
	profile.gamesWon = json["gamesWon"].u();
	profile.hoursPlayed = json["hoursPlayed"].d();
	profile.avatarPath = json["avatarPath"].s();
	return profile;
}

bool ApiClient::DownloadAvatar(unsigned playerId, const std::string& localPath)
{
	const std::string url = m_serverURL + "/users/" + std::to_string(playerId) + "/avatar";

	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		kTimeout
	);

	if (response.error || response.status_code != 200)
		return false;

	std::ofstream out(localPath, std::ios::binary);
	if (!out.is_open()) return false;

	out.write(response.text.data(), response.text.size());
	return true;
}


bool ApiClient::UpdateAvatar(const std::string& imagePath)
{
	unsigned playerId;
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_playerId == 0u) return false;
		playerId = m_playerId;
	}

	std::ifstream file(imagePath, std::ios::binary);
	if (!file.is_open()) return false;
	std::string buffer{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
	if (buffer.empty()) return false;

	const std::string url = m_serverURL + "/users/" + std::to_string(playerId) + "/avatar";
	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body(buffer),
		cpr::Header{ {"Content-Type", "image/png"}, {"Content-Length", std::to_string(buffer.size())}},
		kTimeout
	);

	return response.status_code == 200;
}

std::vector<std::string> ApiClient::GetLobbyPlayers(const std::string& lobbyId) const
{
	const std::string url = m_serverURL + "/lobby/" + lobbyId + "/players";
	cpr::Response response = cpr::Get(cpr::Url{ url }, kTimeout);

	std::vector<std::string> players;
	if (response.error || response.status_code != 200) return players;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("players")) return players;

	for (const auto& player : json["players"])
	{
		players.emplace_back(player.s());
	}
	return players;
}

std::optional<std::vector<uint8_t>> ApiClient::GetPlayerCards() const
{
	const std::string url = m_serverURL + "/lobby/" + m_lobbyId + "/players/" +
		std::to_string(m_playerId) + "/cards";
	cpr::Response response = cpr::Get(cpr::Url{ url }, kTimeout);
	if (response.error || response.status_code != 200) return std::nullopt;

	std::vector<uint8_t> playerCards;
	auto json = crow::json::load(response.text);
	if (!json || !json.has("cards")) return std::nullopt;

	for (const auto& card : json["cards"])
	{
		playerCards.emplace_back(static_cast<uint8_t>(card.i()));
	};
	return playerCards;
}

std::optional<std::array<bool, 4>> ApiClient::GetValidDecks(uint8_t cardId) const
{
	const std::string url{ m_serverURL + "/lobby/" + m_lobbyId
		+ "/decks/valid" };
	const std::string jsonBody{ "{\"cardId\":\"" + std::to_string(cardId) + "\"}" };
	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("decks")) return std::nullopt;

	std::array<bool, 4> validDecks;
	uint8_t deckIndex{ 0u };
	for (const auto& deck : json["decks"])
	{
		validDecks[deckIndex++] = deck.b();
	}
	return std::make_optional(validDecks);
}

bool ApiClient::IsGameActive() const
{
	return m_isGameActive.load();
}

std::optional<bool> ApiClient::GetCanPlayMore(uint8_t remainingCards) const
{
	const std::string url{ m_serverURL + "/lobby/" + m_lobbyId +
	"/players/" + std::to_string(m_playerId) + "/can_play" };
	const std::string jsonBody{ "{\"remaining_cards\":\""
		+ std::to_string(remainingCards) + "\"}" };

	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("can_play")) return std::nullopt;

	return json["can_play"].b();
}

std::optional<std::vector<uint8_t>> ApiClient::EndTurn()
{
	if (m_lobbyId.empty() || m_playerId == 0u) return std::nullopt;

	const std::string url{ m_serverURL + "/lobby/" + m_lobbyId +
		"/players/" + std::to_string(m_playerId) + "/endturn" };
	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		kTimeout
	);
	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("cards")) return std::nullopt;

	std::vector<uint8_t> drawnCards;
	for (const auto& card : json["cards"])
	{
		drawnCards.emplace_back(static_cast<uint8_t>(card.i()));
	}
	return std::make_optional(drawnCards);
}

std::optional<TurnState> ApiClient::GetTurnState() const
{
	const std::string url{ m_serverURL + "/lobby/" + m_lobbyId
		+ "/players/" + std::to_string(m_playerId) + "/turn" };
	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		kTimeout
	);
	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json
		|| !json.has("isMyTurn")
		|| !json.has("currentPlayerId")
		|| !json.has("currentUsername")) return std::nullopt;

	TurnState state;
	state.isMyTurn = json["isMyTurn"].b();
	state.currentPlayerId = static_cast<unsigned>(json["currentPlayerId"].i());
	state.currentUsername = json["currentUsername"].s();
	return state;
}

std::optional<std::vector<std::pair<uint8_t, uint8_t>>> ApiClient::GetAICards(uint8_t cardsRequired) const
{
	const std::string url{ m_serverURL + "/lobby/" + m_lobbyId
	+ "/players/" + std::to_string(m_playerId) + "/place-cards-AI" };
	const std::string jsonBody{ "{\"cards_required\":\""
	+ std::to_string(cardsRequired) + "\"}" };

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("card_id") || !json.has("deck_id")) return std::nullopt;

	const auto& cardIds = json["card_id"];
	const auto& deckIds = json["deck_id"];
	if (cardIds.size() != deckIds.size()) return std::nullopt;

	std::vector<std::pair<uint8_t, uint8_t>> cards;
	cards.reserve(cardsRequired);

	for (size_t i = 0; i < cardIds.size(); ++i)
	{
		cards.emplace_back(
			static_cast<uint8_t>(cardIds[i].i()),
			static_cast<uint8_t>(deckIds[i].i())
		);
	}
	return cards;
}

std::optional<GameResult> ApiClient::ParseGameResult(const std::string& result) const
{
	if (result == "ongoing") return GameResult::Ongoing;
	if (result == "won") return GameResult::Won;
	if (result == "lost") return GameResult::Lost;
	if (result == "NOT_ENOUGH_PLAYERS") return GameResult::NotEnoughPlayers;
	return std::nullopt;
}

std::optional<GameResult> ApiClient::GetGameResult(const std::string& lobbyId) const
{
	const std::string url = m_serverURL + "/lobby/" + lobbyId + "/game/result";
	cpr::Response response = cpr::Get
	(cpr::Url{ url },
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("result")) return std::nullopt;

	auto result = ParseGameResult(json["result"].s());
	if (!result) return std::nullopt;
	return result;
}

std::optional<crow::json::rvalue> ApiClient::GetLobbyInfo(const std::string& lobbyId) const
{
	const std::string url{ m_serverURL + "/lobby/" + lobbyId
	+ "/info" };
	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	return crow::json::load(response.text);
}

void ApiClient::StartNetworkLoop()
{
	StopNetworkLoop();

	m_netThread = std::jthread([this](std::stop_token st) {
		auto lastPing = std::chrono::steady_clock::now();
		auto lastLobby = std::chrono::steady_clock::now();
		auto lastPlayers = std::chrono::steady_clock::now();

		auto lastChat = std::chrono::steady_clock::now();
		auto lastDecks = std::chrono::steady_clock::now();
		auto lastEndTurn = std::chrono::steady_clock::now();
		auto lastGameTabelState = std::chrono::steady_clock::now();
		auto lastTurnState = std::chrono::steady_clock::now();
		auto lastGameResult = std::chrono::steady_clock::now();
		auto lastPlayerStates = std::chrono::steady_clock::now();
		auto lastCanStillPlay = std::chrono::steady_clock::now();

		while (!st.stop_requested())
		{
			std::string lobbyId;
			unsigned playerId;
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				lobbyId = m_lobbyId;
				playerId = m_playerId;
			}
			if (lobbyId.empty() || playerId == 0u)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}

			auto now = std::chrono::steady_clock::now();

			if (now - lastPing >= std::chrono::seconds(10))
			{
				SendPing(lobbyId, playerId);
				lastPing = now;
			}
			if (now - lastPlayers >= std::chrono::seconds(1))
			{
				auto players = GetLobbyPlayers(lobbyId);
				{
					std::lock_guard<std::mutex> lk(m_cacheMutex);
					m_cachedPlayers = std::move(players);
				}
				lastPlayers = now;
			}
			if (now - lastLobby >= std::chrono::seconds(1))
			{
				auto info = GetLobbyInfo(lobbyId);
				if (info && info->has("is_game_started") && (*info)["is_game_started"].b())
				{
					m_event.store(ClientEvent::MatchStarted);
				}
				lastLobby = now;
			}

			if (m_isGameActive.load())
			{
				if (now - lastChat >= std::chrono::milliseconds(200))
				{
					unsigned after{ m_afterChatId.load() };
					auto messages = GetNewMessages(after);
					std::lock_guard<std::mutex> lk(m_cacheMutex);
					if (messages && !messages->empty())
					{
						unsigned maxId{ after };
						for (auto& message : messages.value())
						{
							maxId = (std::max)(maxId, message.id);
						}
						m_afterChatId.store(maxId);
						m_cachedChat = std::move(messages);
					}
					else
					{
						m_cachedChat.reset();
					}
					lastChat = now;
				}
				if (now - lastDecks >= std::chrono::milliseconds(500))
				{
					auto decks = GetDecksState();
					bool decksChanged{ false };
					{
						std::lock_guard<std::mutex> lk(m_cacheMutex);
						if (!m_cachedDecks.has_value() && decks.has_value())
						{
							decksChanged = true;
						}
						else if (m_cachedDecks.has_value() && decks.has_value())
						{
							if (m_cachedDecks->size() != decks->size())
							{
								decksChanged = true;
							}
							else
							{
								for (size_t i = 0; i < decks->size(); ++i)
								{
									if (m_cachedDecks.value()[i].topCardId != decks.value()[i].topCardId)
									{
										decksChanged = true;
										break;
									}
								}
							}
						}
						m_cachedDecks = decks;
						if (decksChanged)
						{
							auto playerCards = GetPlayerCards();
							if (playerCards)
							{
								m_cachedValidDecks.clear();
								for (uint8_t card : playerCards.value())
								{
									auto validDecks = GetValidDecks(card);
									m_cachedValidDecks[card] = validDecks;
								}
							}
						}
					}
					lastDecks = now;
				}
				if (now - lastEndTurn >= std::chrono::milliseconds(500))
				{
					auto endTurn = GetEndTurnState();
					{
						std::lock_guard<std::mutex> lk(m_cacheMutex);
						m_cachedEndTurnState = endTurn;
					}
					lastEndTurn = now;
				}
				if (now - lastGameTabelState >= std::chrono::milliseconds(500))
				{
					auto gameTableState = GetTableState(lobbyId);
					{
						std::lock_guard<std::mutex> lk(m_cacheMutex);
						m_cachedGameTableState = gameTableState;
					}
					lastGameTabelState = now;
				}
				if (now - lastTurnState >= std::chrono::milliseconds(500))
				{
					auto turnState = GetTurnState();
					{
						std::lock_guard<std::mutex> lk(m_cacheMutex);
						m_cachedTurnState = turnState;
					}
					lastTurnState = now;
				}
				if (now - lastGameResult >= std::chrono::seconds(1))
				{
					auto result = GetGameResult(lobbyId);
					{
						std::lock_guard<std::mutex> lk(m_cacheMutex);
						m_cachedGameResult = result;
					}
					lastGameResult = now;
				}
				if (now - lastPlayerStates >= std::chrono::milliseconds(500))
				{
					auto playerStates = GetPlayerStates();
					{
						std::lock_guard<std::mutex> lk(m_cacheMutex);
						m_cachedPlayerStates = playerStates;
					}
					lastPlayerStates = now;
				}
				if (now - lastCanStillPlay >= std::chrono::milliseconds(500))
				{
					if (m_cachedEndTurnState &&
						m_cachedEndTurnState.value().playerId != m_playerId &&
						m_cachedEndTurnState.value().minRequiredCards == 2 &&
						m_cachedEndTurnState.value().cardsPlaced <
						m_cachedEndTurnState.value().minRequiredCards)
					{
						const uint8_t remainingCards =
							m_cachedEndTurnState.value().minRequiredCards -
							m_cachedEndTurnState.value().cardsPlaced;
						if (remainingCards > 0)
						{
							auto canPlayMore = GetCanPlayMore(remainingCards);
							m_canPlayMore = canPlayMore;
						}
					}
					lastCanStillPlay = now;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
		});
}

void ApiClient::StopNetworkLoop()
{
	if (m_netThread.joinable())
	{
		m_netThread.request_stop();
		m_netThread.join();
	}
}

std::vector<std::string> ApiClient::SnapshotPlayers() const
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	return m_cachedPlayers;
}

std::optional<LobbyState> ApiClient::SnapshotLobbyState() const
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	return m_cachedLobby;
}

std::optional<std::array<bool, 4>> ApiClient::SnapshotValidDecks(uint8_t cardId) const
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	auto it = m_cachedValidDecks.find(cardId);
	if (it != m_cachedValidDecks.end()) return it->second;
	return std::nullopt;
}

std::optional<std::vector<ChatMessage>> ApiClient::SnapshotNewChat()
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	auto result = m_cachedChat;
	m_cachedChat.reset();
	return result;
}

std::optional<std::vector<DeckState>> ApiClient::SnapshotDecks() const
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	return m_cachedDecks;
}

std::optional<GameTableState> ApiClient::GetTableState(const std::string& lobbyId) const
{
	const std::string url{ m_serverURL + "/lobby/" + lobbyId + "/gamestate" };
	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("drawPileCount")) return std::nullopt;

	GameTableState state;
	state.drawPileCount = json["drawPileCount"].i();
	return state;
}

std::optional<EndTurnState> ApiClient::SnapshotEndTurnState() const
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	return m_cachedEndTurnState;
}

std::optional<GameTableState> ApiClient::SnapshotGameTableState() const
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	return m_cachedGameTableState;
}

std::optional<TurnState> ApiClient::SnapshotTurnState() const
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	return m_cachedTurnState;
}

std::optional<GameResult> ApiClient::SnapshotGameResult() const
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	return m_cachedGameResult;
}

std::optional<std::vector<PlayerState>> ApiClient::SnapshotPlayerStates() const
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	return m_cachedPlayerStates;
}

std::optional<bool> ApiClient::SnapshotCanPlayMore() const
{
	std::lock_guard<std::mutex> lk(m_cacheMutex);
	return m_canPlayMore;
}
