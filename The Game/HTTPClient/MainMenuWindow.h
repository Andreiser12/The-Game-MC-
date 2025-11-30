#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include "ExitButton.h"
#include "SettingsButton.h"

enum class UiState
{
	MainMenu,
	ExitMenu,
	SettingsMenu
};

class MainMenuWindow
{
public:
	MainMenuWindow(sf::RenderWindow& window);
	void HandleEvent(sf::Event event);
	void Draw();

	void ExitButtonPressed();
	void SettingsButtonPressed();

private:
	void LoadWidgets();

private:
	sf::RenderWindow& m_window;
	UiState m_uiState = UiState::MainMenu;

	tgui::Gui m_mainMenu;
	tgui::Picture::Ptr m_background;

	ExitButton m_exitLayout;
	SettingsButton m_settingsLayout;
};

