#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include "ExitPanel.h"
#include "SettingsPanel.h"

#include <iostream>
#include <vector>
#include <optional>


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

	tgui::Gui m_mainMenu;
	tgui::Picture::Ptr m_background;
	unsigned m_resolutionIndex = 8u;

	std::vector<sf::Vector2u> m_resolutions = {
			{640, 480},
			{ 800, 600 },
			{ 1024, 768 },
			{ 1280, 720 },
			{ 1280, 800 },
			{ 1366, 768 },
			{ 1440, 900 },
			{ 1680, 1050 },
			{ 1920, 1080 },
			{ 1920, 1200 },
			{ 2560, 1440 }
	};
	std::vector<unsigned> m_textSizes = {
		8u,
		10u,
		12u,
		14u,
		18u,
		16u,
		20u,
		22u,
		24u,
		24u,
		24u
	};
	std::vector<unsigned> m_framerates = {
		30u,
		60u,
		120u,
		144u,
		180u
	};
	unsigned m_framerateIndex{ 1u };
	bool m_isFullscreen{ true };

	/*std::vector<sf::Texture> m_bgFrames;
	std::optional<sf::Sprite> m_bgSprite;
	std::size_t m_bgCurrentFrame = 0;
	sf::Clock m_bgClock;
	const float m_bgFrameDuration = 1.f / 24.f;*/

};

