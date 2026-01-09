#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <SFML/Audio.hpp>

#include "JoinPanel.h"
#include "HostPanel.h"
#include "ApiClient.h"

class PlayPanel
{	
public:
	static tgui::Panel::Ptr Create(tgui::Gui& gui,
		ApiClient& apiClient,
		sf::Sound& btnSound,
		std::function<void()> onClose,
		std::function<void()> onJoin,
		std::function<std::string(ApiClient&, const std::string&)> onHost);

private:

};

