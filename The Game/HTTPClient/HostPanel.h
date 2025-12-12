#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include "ApiClient.h"

class HostPanel
{
public:
	static tgui::Panel::Ptr Create(tgui::Gui& gui,
		ApiClient& apiClient,
		const std::string& lobbyId,
		std::function<bool(ApiClient&)> onLeave,
		std::function<bool(ApiClient&)> onStart);

	static void RefreshPlayersList();
	static void UpdateStartButton();
	static void StopUpdateThread();
	static tgui::ListBox::Ptr GetPlayersListBox() { return playersListBox; }
private:
	static tgui::ListBox::Ptr playersListBox;
	static tgui::Button::Ptr startButton;
	static std::atomic<bool> s_running;
	static std::thread s_updateThread;
	static std::vector<std::string> s_playersBuffer;
	static std::mutex s_playersMutex;
	static int s_hostId;
	static int s_playerId;

};

