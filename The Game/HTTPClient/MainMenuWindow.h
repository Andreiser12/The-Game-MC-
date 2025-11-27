#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include "ExitButton.h"

class MainMenuWindow
{
public:
	MainMenuWindow(sf::RenderWindow& window);
	void HandleEvent(sf::Event event);
	void Draw();

	void ExitButtonPressed();

private:
	void LoadWidgets();

private:
	sf::RenderWindow& m_window;
	tgui::Gui m_mainMenu;
	ExitButton m_exitLayout;

	bool m_showExitMenu{ false };
};

