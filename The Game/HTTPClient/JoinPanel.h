#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include "ApiClient.h"
#include "HostPanel.h"

class JoinPanel
{
public:
	static tgui::Panel::Ptr Create(tgui::Gui& gui,
		ApiClient& apiClient,
		std::function<void()> onBack,
		std::function<bool(ApiClient&, std::string)> onJoinLobby);

	static void StopUpdateThread();

private:
	static std::atomic<bool> s_running;

};

