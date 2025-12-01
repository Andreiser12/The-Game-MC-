#pragma once
#include <SFML/Graphics.hpp>
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include "ExitButton.h"
#include "SettingsButton.h"
#include <vector>

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

	/*std::vector<sf::Texture> m_bgFrames;
	sf::Sprite m_bgSprite;
	std::size_t m_bgCurrentFrame = 0;
	sf::Clock m_bgClock;
	const float m_bgFrameDuration = 1.f / 24.f;*/

};

