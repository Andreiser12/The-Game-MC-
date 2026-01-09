#pragma once
#include "PlayPanel.h"
#include "IWindow.h"
#include "ApiClient.h"
#include "GamePanel.h"
#include "Utils.h"
#include <SFML/Audio.hpp>

#include <iostream>

class MainMenuWindow : IWindow
{
public:
	MainMenuWindow(sf::RenderWindow& window, ApiClient& api);
	void HandleEvent(sf::Event event) override;
	void Draw() override;
	void Update();

	void ExitButtonPressed();
	void SettingsButtonPressed();
	void ProfileButtonPressed();
	void PlayPanelPressed();
	void CloseHostPanel();
	void ShutdownLobby();

private:
	void LoadWidgets() override;
	void GoBackToMenu();
	void HandleLocalPlayerLeft();

private:
	sf::RenderWindow& m_window;
	tgui::Gui m_mainMenu;
	tgui::Picture::Ptr m_background;
	std::shared_ptr<GamePanel> m_gamePanel;
	sf::Clock m_lobbyPollCheck;
	bool m_gameStartedLocally{ false };

	unsigned m_resolutionIndex{ 8u };
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
	ApiClient& m_apiClient;

	sf::Music m_bgMusic;
	sf::SoundBuffer m_clickBuffer;
	sf::Sound m_clickSound;
	float m_masterVolume{ 100.0f };
	float m_buttonsVolume{ 100.0f };
	float m_backgroundVolume{ 100.0f };
};

