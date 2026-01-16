module;
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <functional>
#include <string>
#include <atomic>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

export module JoinPanel;

import ApiClient;

export class JoinPanel
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

