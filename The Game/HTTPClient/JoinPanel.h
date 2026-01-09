#pragma once
#include "ApiClient.h"
#include "HostPanel.h"
#include "Utils.h"

class JoinPanel
{
public:
	static tgui::Panel::Ptr Create(tgui::Gui& gui,
		ApiClient& apiClient,
		sf::Sound& btnSound,
		std::function<void()> onBack,
		std::function<bool(ApiClient&, std::string)> onJoinLobby);

	static void StopUpdateThread();

private:
	static std::atomic<bool> s_running;

};

