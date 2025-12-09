#include "APIClient.h"

ApiClient::ApiClient(const std::string& serverURL) : m_serverURL{serverURL} {}

std::optional<std::string> ApiClient::CreateLobby(const std::string& username)
{
	const std::string url = m_serverURL + "/lobby/create";
	std::string jsonBody = "{\"username\":\"" + username + "\"}";

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

std::optional<unsigned> ApiClient::JoinLobby(const std::string& lobbyId,
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

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json || json["status"].s() != "OK") return std::nullopt;
	return json["playerId"].i();
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

	std::string text = response.text;

	text.erase(std::remove_if(text.begin(), text.end(), ::isspace), text.end());

	if (text == "OK" || text == "DELETED") return true;

	return false;
}

std::optional<std::string> ApiClient::GetLobbyState(const std::string& lobbyId)
{
	const std::string url = m_serverURL + "/lobby/info/" + lobbyId;
	cpr::Response response = cpr::Get
		(cpr::Url{ url },
		cpr::Header{{"Content-Type", "application/json"}});

	if (response.error || response.status_code != 200) return std::nullopt;

	auto json = crow::json::load(response.text);
	if (!json) return std::nullopt;

	return json["status"].s();
}

bool ApiClient::StartMatch(const std::string& lobbyId, unsigned playerId)
{
	const std::string url = m_serverURL + "/lobby/start";
	std::string jsonBody =
		"{\"lobbyId\":\"" + lobbyId +
		"\",\"playerId\":" + std::to_string(playerId) +
		"}";

	cpr::Response response = cpr::Post(
		cpr::Url{ url },
		cpr::Body{jsonBody},
		cpr::Header{ {"Content-Type", "application/json"} });

	if (response.error || response.status_code != 200) return false;

	std::string matchStart = response.text;
	matchStart.erase(
		std::remove_if(matchStart.begin(), matchStart.end(), ::isspace), matchStart.end());

	return matchStart == "STARTED";
}

void ApiClient::SetUsername(const std::string& username)
{
	m_username = username;
}

void ApiClient::SetPlayerId(unsigned playerId)
{
	m_playerId.value() = playerId;
}

const std::string& ApiClient::GetUsername() const
{
	return m_username;
}