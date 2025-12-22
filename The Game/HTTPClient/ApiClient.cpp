#include "APIClient.h"

ApiClient::ApiClient(const std::string& serverURL) : m_serverURL{serverURL} {}

static inline cpr::Timeout kTimeout{ 400 };

std::optional<std::string> ApiClient::CreateLobby(const std::string& username)
{
	const std::string url = m_serverURL + "/lobby/create";
	std::string jsonBody = "{\"username\":\"" + username +
		"\",\"playerId\":\"" + std::to_string(m_playerId) + "\"}";

	cpr::Response response = cpr::Post(
		cpr::Url{url},
		cpr::Body{jsonBody},
		cpr::Header{{"Content-Type", "application/json"}},
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json) return std::nullopt;

	const std::string lobbyId = json["lobbyId"].s();

	return lobbyId;
}

bool ApiClient::JoinLobby(const std::string& lobbyId,
	const std::string& username)
{
	const std::string url = m_serverURL + "/lobby/join";

	std::string jsonBody =
		"{\"lobbyId\":\"" + lobbyId +
		"\",\"username\":\"" + username + "\"}";

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		cpr::Header{ {"Content-Type", "application/json"} },
		kTimeout
	);

	if (response.error || response.status_code != 200) return false;

	auto json = crow::json::load(response.text);
	if (!json) return false;

	if (json.has("lobbyId"))
		m_lobbyId = json["lobbyId"].s();

	if (json.has("playerId"))
		m_playerId = json["playerId"].i();

	if (json.has("status") && json["status"].s() == "OK")
		return true;

	return false;
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
		cpr::Body{jsonBody},
		cpr::Header{ {"Content-Type", "application/json"} },
		kTimeout
	);

	if (response.error || response.status_code != 200) return false;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("status"))
		return false;

	if (json["status"].s() == "PLAYER_LEFT")
		m_event.store(ClientEvent::PlayerLeft);
	else if (json["status"].s() == "LOBBY_DELETED")
		m_event.store(ClientEvent::HostLeftLobby);
	else return false;

	m_playerId = 0u;
	m_lobbyId.clear();
	return true;
}
std::optional<LobbyState> ApiClient::GetLobbyState(const std::string& lobbyId)
{
	const std::string url = m_serverURL + "/lobby/info/" + lobbyId;
	cpr::Response response = cpr::Get
		(cpr::Url{ url },
		cpr::Header{{"Content-Type", "application/json"}},
		kTimeout
		);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json) return std::nullopt;

	LobbyState state;
	state.m_status = json["status"].s();
	state.m_message = json["message"].s();
	state.m_hostId = json["hostId"].i();
	return state;
}

bool ApiClient::StartMatch(const std::string& lobbyId, unsigned playerId)
{
	const std::string url = m_serverURL + "/lobby/start";
	const std::string jsonBody =
		"{\"lobbyId\":\"" + lobbyId +
		"\",\"playerId\":" + std::to_string(playerId) +
		"}";

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{jsonBody},
		cpr::Header{ {"Content-Type", "application/json"} },
		kTimeout
	);

	if (response.error || response.status_code != 200) return false;
	auto json = crow::json::load(response.text);
	return json["status"].s() == "GAME_STARTED";
}

bool ApiClient::SendChatMessage(const std::string& message)
{
	const std::string url = m_serverURL + "/lobby/chat/send";
	const std::string jsonBody =
		"{"
		"\"lobbyId\": \"" + m_lobbyId + "\","
		"\"username\": \"" + m_username + "\","
		"\"playerId\": " + std::to_string(m_playerId) + ","
		"\"message\": \"" + message + "\""
		"}";

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		cpr::Header{ {"Content-Type", "application/json"} },
		kTimeout);

	if (response.error || response.status_code != 200) return false;

	return true;
}

std::optional<std::vector<ChatMessage>> ApiClient::GetNewMessages(unsigned afterId)
{
	if (m_lobbyId.empty()) return std::nullopt;

	const std::string url = m_serverURL + "/lobby/" + m_lobbyId
		+ "/chat/messages?after=" + std::to_string(afterId);

	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		cpr::Header{ {"Content-Type", "application/json"} },
		kTimeout
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("messages")) return std::nullopt;

	std::vector<ChatMessage> result;
	for (const auto& message : json["messages"])
	{
		result.emplace_back(
			(unsigned)message["id"].i(),
			message["username"].s(),
			message["message"].s());
	}
	return result;
}

bool ApiClient::PlayCard(uint8_t cardId, uint8_t deckId)
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
		cpr::Header{ {"Content-Type", "application/json"} },
		kTimeout
	);
	if (response.error || response.status_code != 200) return false;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("status")) return false;

	return json["status"] == "OK";
}

std::optional<std::vector<DeckState>> ApiClient::GetDeckState()
{
	const std::string url = m_serverURL + "/lobby/" + m_lobbyId +
		"/decks";
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
		deckStates.emplace_back(deck);
	}
	return std::make_optional(deckStates);
}

bool ApiClient::SendPing(const std::string& lobbyId, unsigned playerId)
{
	const std::string url = m_serverURL + "/lobby/ping";
	std::string jsonBody = "{\"lobbyId\":\"" + lobbyId + "\",\"playerId\":" + std::to_string(playerId) + "}";

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		cpr::Header{ {"Content-Type", "application/json"} },
		kTimeout
	);

	if (response.error || response.status_code != 200) return false;

	auto json = crow::json::load(response.text);
	return json && json["status"].s() == "OK";
}

void ApiClient::StartPingThread(const std::string& lobbyId, unsigned playerId)
{
	StopPingThread();

	m_pingThread = std::jthread([this, lobbyId, playerId](std::stop_token st)
		{
			while (!st.stop_requested())
			{
				std::this_thread::sleep_for(std::chrono::seconds(5));
				SendPing(lobbyId, playerId);
			}
		});
}

void ApiClient::StopPingThread()
{
	m_pingThread.request_stop();
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

void ApiClient::StartLobbyWatcher()
{
	m_lobbyWatcher = std::jthread([this](std::stop_token st)
		{
			while (!st.stop_requested())
			{
				std::string lobbyId = GetLobbyId();
				
				if (lobbyId.empty()) return;
				auto lobby = GetLobbyState(lobbyId);
				if (!lobby || lobby->m_status == "LOBBY_DELETED")
				{
					m_event.store(ClientEvent::LobbyClosed);
					return;
				}
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		});
}

void ApiClient::StopLobbyWatcher()
{
	if (m_lobbyWatcher.joinable())
	{
		m_lobbyWatcher.request_stop();
		m_lobbyWatcher.join();
	}
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

std::optional<std::vector<unsigned>> ApiClient::GetPlayerCards() const
{
	const std::string url = m_serverURL + "/lobby/" + m_lobbyId + "/players/" +
		std::to_string(m_playerId) + "/cards";
	cpr::Response response = cpr::Get(cpr::Url{ url }, kTimeout);
	if (response.status_code != 200) return std::nullopt;

	std::vector<unsigned> playerCards;

	auto json = crow::json::load(response.text);
	if (!json) return std::nullopt;

	for (const auto& card : json["cards"])
	{
		playerCards.push_back(card.i());
	}
	return playerCards;
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

		while (!st.stop_requested())
		{
			if (m_lobbyId.empty() || m_playerId == 0u)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}

			auto now = std::chrono::steady_clock::now();
			if (now - lastPing >= std::chrono::seconds(10))
			{
				SendPing(m_lobbyId, m_playerId);
				lastPing = now;
			}
			if (now - lastLobby >= std::chrono::seconds(2))
			{
				auto lobby = GetLobbyState(m_lobbyId);
				{
					std::lock_guard<std::mutex> lk(m_cacheMutex);
					m_cachedLobby = lobby;
				}
				if (!lobby || lobby->m_status == "LOBBY_DELETED")
				{
					m_event.store(ClientEvent::LobbyClosed);
					return;
				}
				lastLobby = now;
			}
			if (now - lastPlayers >= std::chrono::milliseconds(500))
			{
				auto players = GetLobbyPlayers(m_lobbyId);
				{
					std::lock_guard<std::mutex> lk(m_cacheMutex);
					m_cachedPlayers = std::move(players);
				}
				lastPlayers = now;
			}
			if (now - lastChat >= std::chrono::milliseconds(200))
			{
				unsigned after = m_afterChatId.load();
				auto messages = GetNewMessages(after);
				std::lock_guard<std::mutex> lk(m_cacheMutex);
				if (messages && !messages->empty())
				{
					unsigned maxId = after;
					for (auto& message : messages.value())
					{
						maxId = std::max(maxId, message.id);
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
				auto decks = GetDeckState();
				{
					std::lock_guard<std::mutex> lk(m_cacheMutex);
					m_cachedDecks = decks;
				}
				lastDecks = now;
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
