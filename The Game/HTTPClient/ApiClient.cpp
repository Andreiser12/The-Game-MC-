#include "APIClient.h"

ApiClient::ApiClient(const std::string& serverURL) : m_serverURL{serverURL} {}

std::optional<std::string> ApiClient::CreateLobby(const std::string& username)
{
	const std::string url = m_serverURL + "/lobby/create";
	std::string jsonBody = "{\"username\":\"" + username +
		"\",\"playerId\":\"" + std::to_string(m_playerId) + "\"}";

	cpr::Response response = cpr::Post(
		cpr::Url{url},
		cpr::Body{jsonBody},
		cpr::Header{{"Content-Type", "application/json"}}
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
		cpr::Header{ {"Content-Type", "application/json"} });

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
		cpr::Header{ {"Content-Type", "application/json"} });

	if (response.error || response.status_code != 200) return false;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("status"))
		return false;

	const std::string status = json["status"].s();
	if (status == "PLAYER_LEFT")
		m_event.store(ClientEvent::PlayerLeft);
	else if (status == "LOBBY_DELETED")
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
		cpr::Header{{"Content-Type", "application/json"}});

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
		cpr::Header{ {"Content-Type", "application/json"} });

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
		cpr::Header{ {"Content-Type", "application/json"} });

	if (response.error || response.status_code != 200) return false;

	return true;
}

std::optional<std::vector<ChatMessage>> ApiClient::GetNewMessages(unsigned afterId)
{
	static std::chrono::steady_clock::time_point lastRequest;
	static std::optional < std::vector<ChatMessage>> cachedMessages;

	auto now = std::chrono::steady_clock::now();

	if (now - lastRequest < std::chrono::milliseconds(50) && cachedMessages) return cachedMessages;

	const std::string url = m_serverURL + "/lobby/" + m_lobbyId
		+ "/chat/messages?after=" + std::to_string(afterId);
	cpr::Response response = cpr::Get(
		cpr::Url{ url },
		cpr::Header{ {"Content-Type", "application/json"} },
		cpr::Timeout{ 500 }
	);

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || !json.has("messages")) return std::nullopt;

	std::vector<ChatMessage> result;
	auto messagesJson = json["messages"];
	for (const auto& message : messagesJson)
	{
		result.emplace_back(
			message["id"].i(),
			message["username"].s(),
			message["message"].s());
	}

	lastRequest = now;
	cachedMessages = result.empty() ? std::nullopt : std::make_optional(result);
	return cachedMessages;
}

bool ApiClient::SendPing(const std::string& lobbyId, unsigned playerId)
{
	const std::string url = m_serverURL + "/lobby/ping";
	std::string jsonBody = "{\"lobbyId\":\"" + lobbyId + "\",\"playerId\":" + std::to_string(playerId) + "}";

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{ jsonBody },
		cpr::Header{ {"Content-Type", "application/json"} }
	);

	if (response.error || response.status_code != 200)
		return false;

	auto json = crow::json::load(response.text);
	if (!json) return false;

	return json["status"].s() == "OK";
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

const std::string& ApiClient::GetUsername() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_username;
}

unsigned ApiClient::GetPlayerId() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_playerId;
}

const std::string& ApiClient::GetLobbyId() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_lobbyId;
}

std::vector<std::string> ApiClient::GetLobbyPlayers(const std::string& lobbyId) const
{
	auto url = m_serverURL + "/lobby/" + lobbyId + "/players";
	cpr::Response response = cpr::Get(cpr::Url{ url });
	std::vector<std::string> players;

	if (response.status_code != 200) return players;

	auto json = crow::json::load(response.text);
	if (!json) return players;

	for (const auto& p : json["players"])
	{
		players.push_back(p.s());
	}
	return players;
}