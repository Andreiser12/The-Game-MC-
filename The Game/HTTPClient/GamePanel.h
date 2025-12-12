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
	void Update();

private:
	unsigned m_lastMessageId{ 0u };
	tgui::Group::Ptr m_root;
	tgui::ChatBox::Ptr m_chat;
	sf::Clock m_chatClock;
	ApiClient& m_apiClient;

};

