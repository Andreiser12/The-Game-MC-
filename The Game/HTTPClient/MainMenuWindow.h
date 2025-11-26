#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

class MainMenuWindow
{
public:
	MainMenuWindow(sf::RenderWindow& window);
	void HandleEvent(sf::Event event);
	void Draw();

private:
	void LoadWidgets();

private:
	tgui::Gui m_mainMenu;
};

