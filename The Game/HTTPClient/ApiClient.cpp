#include "APIClient.h"

ApiClient::ApiClient(const std::string& serverURL) : m_serverURL{serverURL} {}

std::optional<std::string> ApiClient::CreateLobby()
{
	const std::string url = m_serverURL + "/lobby/create";

	cpr::Response response = cpr::Post(cpr::Url{ url });

	if (response.error || response.status_code != 200) return std::nullopt;

	std::string lobbyCode = response.text;
	lobbyCode.erase(
		std::remove_if(lobbyCode.begin(), lobbyCode.end(),
			[](unsigned char c) { return std::isspace(c); })
	);


	if (lobbyCode.empty()) return std::nullopt;

	return lobbyCode;
}

std::optional<std::string> ApiClient::JoinLobby(const std::string& lobbyCode)
{
	const std::string url = m_serverURL + "/lobby/join/" + lobbyCode;
	cpr::Response response = cpr::Post(cpr::Url{ url }, 
		cpr::Body{ "lobby_code=" + lobbyCode});


	if (response.error || response.status_code != 200) return std::nullopt;

	std::string lobbyId = response.text;

	lobbyId.erase(
		std::remove_if(lobbyCode.begin(), lobbyCode.end(),
			[](unsigned char c) { return std::isspace(c); })
	);

	if (lobbyId.empty()) return std::nullopt;

	return lobbyId;
}

bool ApiClient::LeaveLobby(const std::string& lobbyId, unsigned playerId)
{
	const std::string url = m_serverURL + "/lobby/leave/" + lobbyId;
	cpr::Response response = cpr::Post(cpr::Url{ url },
		cpr::Payload{ {"playerId", std::to_string(playerId)} });

	if (response.error || response.status_code != 200) return false;

	return true;
}

std::optional<std::string> ApiClient::GetLobbyState(const std::string& lobbyId)
{
	
}

bool ApiClient::StartMatch(unsigned lobbyId)
{
	return false;
}
