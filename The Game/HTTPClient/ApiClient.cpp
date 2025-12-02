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

std::optional<std::string> ApiClient::JoinLobby(unsigned lobbyCode)
{
	return std::optional<std::string>();
}

bool ApiClient::LeaveLobby(unsigned lobbyId)
{
	return false;
}

std::optional<std::string> ApiClient::GetLobbyState(unsigned lobbyId)
{
	return std::optional<std::string>();
}

bool ApiClient::StartMatch(unsigned lobbyId)
{
	return false;
}
