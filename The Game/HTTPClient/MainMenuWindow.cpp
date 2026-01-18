module;
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <filesystem>

module MainMenuWindow;

import HostPanel;

MainMenuWindow::MainMenuWindow(sf::RenderWindow& window, ApiClient& api) :
	m_window{ window },
	m_mainMenu{ window },
	m_apiClient{ api },
	m_clickSound{ m_clickBuffer }
{
	m_mainMenu.setFont("assets/fonts/Cinzel-Regular.ttf");
	m_background = tgui::Picture::create("assets/images/mainMenuBackground.png");
	m_background->setSize({ "100%", "100%" });
	m_background->setPosition("0%", "0%");
	m_mainMenu.add(m_background);

	auto buttonsBackground = tgui::Picture::create("assets/images/buttonsBackground.png");
	buttonsBackground->setSize({ "90%", "90%" });
	buttonsBackground->setPosition("50%", "52.5%");
	buttonsBackground->setOrigin(0.5f, 0.5f);
	m_mainMenu.add(buttonsBackground);

	auto cardsImage = tgui::Picture::create("assets/images/cardsMainMenuImage.png");
	cardsImage->setSize({ "67%", "67%" });
	cardsImage->setPosition("80%", "60%");
	cardsImage->setOrigin(0.5f, 0.5f);
	cardsImage->getRenderer()->setOpacity(0.95f);

	auto cardsShadow = tgui::Picture::create("assets/images/cardsMainMenuImage.png");
	cardsShadow->setSize({ "67%", "67%" });
	cardsShadow->setPosition("80% + 5", "60% + 5");
	cardsShadow->setOrigin(0.5f, 0.5f);
	cardsShadow->getRenderer()->setOpacity(0.3f);
	m_mainMenu.add(cardsShadow);
	m_mainMenu.add(cardsImage);

	auto gameTitle = tgui::Picture::create("assets/images/gameTitle.png");
	gameTitle->setSize({ "50%", "50%" });
	gameTitle->setPosition("50%", "15%");
	gameTitle->setOrigin(0.5f, 0.1f);
	m_mainMenu.add(gameTitle);

	LoadWidgets();

	if (m_bgMusic.openFromFile("assets/sounds/main_menu_sound.ogg"))
	{
		m_bgMusic.setLooping(true);
		m_bgMusic.setVolume(50.0f);
		m_bgMusic.play();
	}

	if (m_clickBuffer.loadFromFile("assets/sounds/press_button_sound.wav"))
	{
		m_clickSound.setVolume(50.0f);
	}
}

void MainMenuWindow::LoadWidgets()
{
	auto playButton = tgui::Button::create();
	playButton->getRenderer()->setTexture("assets/images/buttons.png");
	playButton->setSize({ "14%", "5%" });
	playButton->setPosition("50%", "50%");
	playButton->setOrigin(0.5f, 0.5f);
	playButton->setText("Play");
	Utils::StyleButton(playButton);
	m_mainMenu.add(playButton);
	playButton->onClick([this] {
		m_clickSound.play();
		PlayPanelPressed(); });

	auto profileButton = tgui::Button::create();
	profileButton->setSize({ "14%", "5%" });
	profileButton->setPosition("50%", "60%");
	profileButton->setOrigin(0.5f, 0.5f);
	profileButton->setText("Profile");
	profileButton->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(profileButton);
	profileButton->onClick([this] {
		m_clickSound.play();
		ProfileButtonPressed(); });
	m_mainMenu.add(profileButton);

	auto settingsButton = tgui::Button::create();
	settingsButton->setSize({ "14%", "5%" });
	settingsButton->setPosition("50%", "70%");
	settingsButton->setOrigin(0.5f, 0.5f);
	settingsButton->setText("Settings");
	settingsButton->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(settingsButton);
	m_mainMenu.add(settingsButton);
	settingsButton->onClick([this] {
		m_clickSound.play();
		SettingsButtonPressed(); });

	auto exitButton = tgui::Button::create();
	exitButton->setSize({ "14%", "5%" });
	exitButton->setPosition("50%", "80%");
	exitButton->setOrigin(0.5f, 0.5f);
	exitButton->setText("Exit");
	exitButton->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(exitButton);
	m_mainMenu.add(exitButton);
	exitButton->onClick([this] {
		m_clickSound.play();
		ExitButtonPressed(); });
}

void MainMenuWindow::GoBackToMenu()
{
	m_apiClient.StopNetworkLoop();
	m_apiClient.SetGameActive(false);

	if (!m_apiClient.GetLobbyId().empty() && m_apiClient.GetPlayerId() != 0u)
		m_apiClient.LeaveLobby(m_apiClient.GetLobbyId(), m_apiClient.GetPlayerId());

	CloseHostPanel();

	if (m_mainMenu.get("GameRoot")) m_mainMenu.remove(m_mainMenu.get("GameRoot"));
	if (m_gamePanel) m_gamePanel.reset();
	m_gameStartedLocally = false;
	m_apiClient.ClearEvent();
	m_apiClient.SetLobbyId("");

	if (m_bgMusic.getStatus() != sf::SoundSource::Status::Playing) m_bgMusic.play();
}

void MainMenuWindow::ApplySettings()
{
	m_clickSound.play();
	const float masterMult{ m_settings.masterVolume / 100.f };

	if (m_bgMusic.getStatus() != sf::SoundSource::Status::Stopped)
	{
		m_bgMusic.setVolume(m_settings.backgroundVolume * masterMult);
	}

	m_clickSound.setVolume(m_settings.buttonsVolume * masterMult);

	const auto& resolution = SettingsState::Resolutions[m_settings.resolutionIndex];

	if (m_settings.fullscreen)
	{
		m_window.create(
			sf::VideoMode({ resolution.x, resolution.y }),
			"The Game 1.0.0",
			sf::State::Fullscreen
		);
	}
	else
	{
		m_window.create(
			sf::VideoMode({ resolution.x, resolution.y }),
			"The Game 1.0.0",
			sf::State::Windowed
		);
	}
	m_window.setFramerateLimit(SettingsState::Framerates[m_settings.framerateIndex]);
	m_mainMenu.setTarget(m_window);
	m_mainMenu.setTextSize(m_textSizes[m_settings.resolutionIndex]);
}

void MainMenuWindow::HandleLocalPlayerLeft()
{
	if (!m_gamePanel) return;

	if (m_mainMenu.get("GameRoot"))
		m_mainMenu.remove(m_mainMenu.get("GameRoot"));

	m_apiClient.ClearEvent();
	m_gamePanel.reset();
	m_gameStartedLocally = false;
	CloseHostPanel();
	m_apiClient.SetLobbyId("");

	if (m_bgMusic.getStatus() != sf::SoundSource::Status::Playing) m_bgMusic.play();
}

void MainMenuWindow::TransitionToGame()
{
	if (m_gameStartedLocally) return;

	m_bgMusic.stop();

	auto hostPanel = m_mainMenu.get<tgui::Panel>("HostPanel");
	if (hostPanel)
	{
		m_mainMenu.remove(hostPanel);
	}

	m_apiClient.SetGameActive(true);
	m_gamePanel = std::make_shared<GamePanel>(m_mainMenu, m_apiClient,
		[this]() { GoBackToMenu(); },
		[this]() {
			m_clickSound.play();
			HandleLocalPlayerLeft();
		});

	m_gamePanel->OnGameStarted();
	m_gameStartedLocally = true;
}

void MainMenuWindow::Draw()
{
	m_mainMenu.draw();
}

void MainMenuWindow::Update()
{
	if (m_gamePanel)
	{
		m_gamePanel->UpdateGame();
		return;
	}
	if (m_gameStartedLocally ||
		m_lobbyPollCheck.getElapsedTime().asMilliseconds() < 500) return;

	m_lobbyPollCheck.restart();
	auto event = m_apiClient.GetEvent();
	switch (event)
	{
	case ClientEvent::PlayerLeft:
		HostPanel::RefreshPlayersList(m_apiClient);
		HostPanel::UpdateStartButton();
		m_apiClient.ClearEvent();
		break;

	case ClientEvent::LobbyClosed:
	case ClientEvent::HostLeftLobby:
	{
		ShutdownLobby();
		m_mainMenu.add(Utils::CreateErrorPopupPanel(m_mainMenu, "Lobby was closed",
			m_clickSound, []() {}));
		return;
	}
	case ClientEvent::MatchStarted:
		TransitionToGame();
		return;
	default:
		break;
	}
}

void MainMenuWindow::ExitButtonPressed()
{
	auto exitPanel = Utils::CreateExitPanel(
		m_mainMenu,
		m_clickSound,
		[this]() {
			m_clickSound.play();
			m_clickSound.play();
			m_apiClient.Logout();
			m_window.close();
		},
		[this]() {
			m_clickSound.play();
			auto panel = m_mainMenu.get<tgui::Panel>("ExitPanel");
			if (panel) m_mainMenu.remove(panel);
		}
	);
	m_mainMenu.add(exitPanel);
}

void MainMenuWindow::SettingsButtonPressed()
{
	auto settingsPanel = Utils::CreateSettingsPanel(
		m_mainMenu,
		m_settings,
		m_clickSound,
		[this]()
		{
			m_clickSound.play();
		},
		[this]()
		{
			ApplySettings();
		});
	m_mainMenu.add(settingsPanel);
}

void MainMenuWindow::OpenProfilePanel()
{
	auto userProfileExists = m_apiClient.GetUserProfile(m_apiClient.GetPlayerId());
	if (!userProfileExists)
	{
		m_mainMenu.add(Utils::CreateErrorPopupPanel(m_mainMenu, "Could not load profile", m_clickSound));
		return;
	}
	if (auto oldPanel = m_mainMenu.get<tgui::Panel>("ProfilePanel"))
		m_mainMenu.remove(oldPanel);
	auto panel = Utils::CreateProfilePanel(
		userProfileExists.value(),
		m_clickSound,
		[this]()
		{
			m_clickSound.play();
			auto panel = m_mainMenu.get<tgui::Panel>("ProfilePanel");
			if (panel) m_mainMenu.remove(panel);
		},
		[this]()
		{
			m_clickSound.play();
			auto confirmPanel = Utils::CreateDeleteAccountPanel(
				m_mainMenu,
				m_clickSound,
				[this]()
				{
					m_clickSound.play();
					m_apiClient.DeleteAccount();
					m_window.close();
				},
				[]() {}
			);
			m_mainMenu.add(confirmPanel);
		},
		[this]()
		{
			OpenAvatarFileDialog();
		}
	);
	m_mainMenu.add(panel, "ProfilePanel");
}

void MainMenuWindow::OpenAvatarFileDialog()
{
	m_clickSound.play();
	auto fileDialog = tgui::FileDialog::create("Select avatar", "Open");
	fileDialog->setFileMustExist(true);
	fileDialog->setSelectingDirectory(false);
	fileDialog->setMultiSelect(false);
	fileDialog->setFileTypeFilters({ {"PNG Images", {"*.png"}} });
	fileDialog->setPath("assets/avatars");
	fileDialog->onFileSelect(
		[this, fileDialog](const std::vector<tgui::Filesystem::Path>& paths)
		{
			if (paths.empty()) return;
			fileDialog->setEnabled(false);
			fileDialog->close();
			std::filesystem::path filePath(paths[0].asNativeString());
			std::string avatarPath = filePath.string();
			if (!m_apiClient.UpdateAvatar(avatarPath))
			{
				m_mainMenu.add(Utils::CreateErrorPopupPanel(m_mainMenu, "Failed to update avatar", m_clickSound));
				return;
			}	
			std::filesystem::path avatarsDirectory = "assets/avatars";
			std::filesystem::create_directories(avatarsDirectory);

			std::string username = m_apiClient.GetUsername();
			std::filesystem::path localCopy = avatarsDirectory / (username + ".png");
			std::filesystem::copy_file(filePath, localCopy, std::filesystem::copy_options::overwrite_existing);
			OpenProfilePanel();
		}
	);
	m_mainMenu.add(fileDialog);
}

void MainMenuWindow::ProfileButtonPressed()
{
	OpenProfilePanel();
}

void MainMenuWindow::PlayPanelPressed()
{
	auto playPanel = Utils::CreatePlayPanel(
		m_mainMenu,
		m_apiClient,
		m_clickSound,
		[this]() { m_clickSound.play(); },
		[this]() { m_clickSound.play(); },
		[this](ApiClient& api, const std::string& user) {
			auto lobbyId = api.CreateLobby(user);
			return lobbyId ? lobbyId.value() : "ERROR";
		},
		[this]() { this->TransitionToGame(); }
	);
	m_mainMenu.add(playPanel);
}

void MainMenuWindow::CloseHostPanel()
{
	auto hostPanel = m_mainMenu.get<tgui::Panel>("HostPanel");
	if (hostPanel) m_mainMenu.remove(hostPanel);
}

void MainMenuWindow::ShutdownLobby()
{
	CloseHostPanel();
	m_apiClient.SetLobbyId("");
	m_apiClient.ClearEvent();
}

void MainMenuWindow::HandleEvent(sf::Event event)
{
	m_mainMenu.handleEvent(event);
	if (m_gamePanel)
	{
		if (event.is<sf::Event::FocusLost>())
		{
			m_gamePanel->ResetAllDrags();
		}
		else
		{
			m_gamePanel->HandleMouseEvent(event);
		}
	}
}