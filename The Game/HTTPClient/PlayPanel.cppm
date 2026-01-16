module;
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

export module PlayPanel;

import JoinPanel;
import HostPanel;
import ApiClient;

export class PlayPanel
{	
public:
	static tgui::Panel::Ptr Create(tgui::Gui& gui,
		ApiClient& apiClient,
		sf::Sound& btnSound,
		std::function<void()> onClose,
		std::function<void()> onJoin,
		std::function<std::string(ApiClient&, const std::string&)> onHost,
		std::function<void()> onTransition);

private:
};

