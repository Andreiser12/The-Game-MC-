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

	static void RefreshPlayersList(ApiClient& apiClient);
	static void UpdateStartButton();
	static inline tgui::ListBox::Ptr GetPlayersListBox() { return playersListBox; }

private:
	static inline tgui::ListBox::Ptr playersListBox{ nullptr };
	static inline tgui::Button::Ptr startButton{ nullptr };
	static inline int s_hostId{ -1 };
	static inline int s_playerId{ -1 };
};

