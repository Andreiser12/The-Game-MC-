#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <SFML/System.hpp>

#include "ApiClient.h"

class GamePanel
{
public:

	GamePanel(tgui::Gui& gui, ApiClient& api);
	static tgui::Group::Ptr Create(tgui::Gui& gui,
		ApiClient& apiClient);
	void Update();

private:
	unsigned m_lastMessageId{ 0 };
	tgui::Group::Ptr m_group;
	sf::Clock m_chatClock;
	ApiClient& m_apiClient;
	tgui::ChatBox::Ptr m_chat;

};

