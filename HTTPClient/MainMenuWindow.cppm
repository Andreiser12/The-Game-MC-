module;
#include <array>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <filesystem>

export module MainMenuWindow;

import IWindow;
import ApiClient;
import GamePanel;
import SettingsState;
import Utils;

export class MainMenuWindow : public IWindow
{
public:
	explicit MainMenuWindow(sf::RenderWindow& window, ApiClient& api);

	MainMenuWindow(const MainMenuWindow&) = delete;
	MainMenuWindow& operator= (const MainMenuWindow&) = delete;
	MainMenuWindow(MainMenuWindow&&) = default;
	MainMenuWindow& operator= (MainMenuWindow&&) = default;

	void HandleEvent(sf::Event event) override;
	void Draw() override;
	void Update();

	void ExitButtonPressed();
	void SettingsButtonPressed();
	void OpenProfilePanel();
	void OpenAvatarFileDialog();
	void ProfileButtonPressed();
	void PlayPanelPressed();
	void CloseHostPanel();
	void ShutdownLobby();

private:
	void LoadWidgets() override;
	void GoBackToMenu();
	void ApplySettings();
	void HandleLocalPlayerLeft();
	void TransitionToGame();

private:
	sf::RenderWindow& m_window;
	tgui::Gui m_mainMenu;
	tgui::Picture::Ptr m_background;
	std::shared_ptr<GamePanel> m_gamePanel;
	sf::Clock m_lobbyPollCheck;
	bool m_gameStartedLocally{ false };
	SettingsState m_settings;

	inline static constexpr std::array<uint8_t, 11> m_textSizes = {
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

	ApiClient& m_apiClient;
	sf::Music m_bgMusic;
	sf::SoundBuffer m_clickBuffer;
	sf::Sound m_clickSound;
};

