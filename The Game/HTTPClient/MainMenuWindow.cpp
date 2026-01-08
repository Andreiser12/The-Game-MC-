#include "MainMenuWindow.h"

MainMenuWindow::MainMenuWindow(sf::RenderWindow& window, ApiClient& api) :
	m_window{ window },
	m_mainMenu{ window },
	m_apiClient{ api }
{
	m_mainMenu.setFont("assets/fonts/Cinzel-Regular.ttf");
	m_background = tgui::Picture::create("assets/images/mainMenuBackground.png");
	m_background->setSize("100%", "100%");
	m_background->setPosition("0%", "0%");
	m_mainMenu.add(m_background);

	auto gameTitle = tgui::Picture::create("assets/images/gameTitle.png");
	gameTitle->setSize("50%", "50%");
	gameTitle->setPosition("50%", "15%");
	gameTitle->setOrigin(0.5f, 0.1f);
	m_mainMenu.add(gameTitle);

	auto buttonsBackground = tgui::Picture::create("assets/images/buttonsBackground.png");
	buttonsBackground->setSize("90%", "90%");
	buttonsBackground->setPosition("50%", "50%");
	buttonsBackground->setOrigin(0.5f, 0.5f);
	m_mainMenu.add(buttonsBackground);

	auto cardsImage = tgui::Picture::create("assets/images/cardsMainMenuImage.png");
	cardsImage->setSize("67%", "67%");
	cardsImage->setPosition("80%", "60%");
	cardsImage->setOrigin(0.5f, 0.5f);
	cardsImage->getRenderer()->setOpacity(0.95f);

	auto cardsShadow = tgui::Picture::create("assets/images/cardsMainMenuImage.png");
	cardsShadow->setSize("67%", "67%");
	cardsShadow->setPosition("80% + 5", "60% + 5");
	cardsShadow->setOrigin(0.5f, 0.5f);
	cardsShadow->getRenderer()->setOpacity(0.3f);
	m_mainMenu.add(cardsShadow);
	m_mainMenu.add(cardsImage);
	LoadWidgets();

	if (m_bgMusic.openFromFile("assets/sounds/13.-Dreamy-Minimal-Piano-and-Jazz-Groove-_B_.ogg"))
	{
		m_bgMusic.setLooping(true);      
		m_bgMusic.setVolume(50.0f);   
		m_bgMusic.play();             
	}
}

void MainMenuWindow::LoadWidgets()
{
	auto playButton = tgui::Button::create();
	playButton->getRenderer()->setTexture("assets/images/buttons.png");
	playButton->setSize("14%", "5%");
	playButton->setPosition("50%", "47.5%");
	playButton->setOrigin(0.5f, 0.5f);
	playButton->setText("Play");
	Utils::StyleButton(playButton);
	m_mainMenu.add(playButton);
	playButton->onClick([this] {PlayPanelPressed(); });

	auto profileButton = tgui::Button::create();
	profileButton->setSize("14%", "5%");
	profileButton->setPosition("50%", "57.5%");
	profileButton->setOrigin(0.5f, 0.5f);
	profileButton->setText("Profile");
	profileButton->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(profileButton);
	profileButton->onClick([this] { ProfileButtonPressed(); });
	m_mainMenu.add(profileButton);

	auto settingsButton = tgui::Button::create();
	settingsButton->setSize("14%", "5%");
	settingsButton->setPosition("50%", "67.5%");
	settingsButton->setOrigin(0.5f, 0.5f);
	settingsButton->setText("Settings");
	settingsButton->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(settingsButton);
	m_mainMenu.add(settingsButton);
	settingsButton->onClick([this] { SettingsButtonPressed(); });

	auto exitButton = tgui::Button::create();
	exitButton->setSize("14%", "5%");
	exitButton->setPosition("50%", "77.5%");
	exitButton->setOrigin(0.5f, 0.5f);
	exitButton->setText("Exit");
	exitButton->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(exitButton);
	m_mainMenu.add(exitButton);
	exitButton->onClick([this] {ExitButtonPressed(); });
}

void MainMenuWindow::GoBackToMenu()
{

	if (!m_apiClient.GetLobbyId().empty() && m_apiClient.GetPlayerId() != 0u)
		m_apiClient.LeaveLobby(m_apiClient.GetLobbyId(), m_apiClient.GetPlayerId());

	m_apiClient.StopNetworkLoop();
	CloseHostPanel();

	if (m_gamePanel) m_gamePanel.reset();
	m_gameStartedLocally = false;
}

void MainMenuWindow::HandleLocalPlayerLeft()
{
	if (!m_gamePanel) return;

	const std::string myUsername = m_apiClient.GetUsername();
	m_gamePanel->MarkPlayerAsLeft(myUsername);

	if (!m_apiClient.GetLobbyId().empty() && m_apiClient.GetPlayerId() != 0u)
	{
		auto result = m_apiClient.LeaveMatch();
		if (result && result.value() == LeaveMatchResult::Success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			m_apiClient.LeaveLobby(m_apiClient.GetLobbyId(), m_apiClient.GetPlayerId());
		}
	}


	m_gamePanel.reset();
	m_gameStartedLocally = false;
	CloseHostPanel();
	m_apiClient.SetLobbyId("");
	m_apiClient.ClearEvent();
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
		return;

	case ClientEvent::LobbyClosed:
	{
		ShutdownLobby();
		m_mainMenu.add(Utils::CreateErrorPopupPanel(m_mainMenu, "Lobby was closed by host.", []() {}));
		return;
	}

	case ClientEvent::HostLeftLobby:
		ShutdownLobby();
		return;

	default:
		break;
	}

	if (m_apiClient.GetLobbyId().empty()) return;
	auto lobbyStateOpt = m_apiClient.GetLobbyState(m_apiClient.GetLobbyId());
	if (!lobbyStateOpt) return;

	if (lobbyStateOpt.value().m_status == "GAME_STARTED" && !m_gamePanel)
	{
		m_bgMusic.stop();

		m_gamePanel = std::make_shared<GamePanel>(m_mainMenu, m_apiClient,
			[this]() { GoBackToMenu(); }, [this]() {HandleLocalPlayerLeft(); });
		m_gamePanel->OnGameStarted();
		m_gameStartedLocally = true;
	}
}

void MainMenuWindow::ExitButtonPressed()
{
	auto exitPanel = Utils::CreateExitPanel(
		m_mainMenu,
		[this]() {
			m_apiClient.Logout();
			m_window.close();
		},
		[this]() {
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
		m_resolutionIndex,
		m_isFullscreen,
		m_framerateIndex,
		[this]() {},
		[this](unsigned newResolutionIndex, bool fullscreen, unsigned newFramerateIndex)
		{
			m_resolutionIndex = newResolutionIndex;
			m_isFullscreen = fullscreen;
			m_framerateIndex = newFramerateIndex;
			if (m_isFullscreen)
			{
				m_window.create(sf::VideoMode(m_resolutions[m_resolutionIndex]),
					"The Game 1.0.0",
					sf::State::Fullscreen);
			}
			else
			{
				m_window.create(sf::VideoMode(m_resolutions[m_resolutionIndex]),
					"The Game 1.0.0",
					sf::State::Windowed);
			}
			m_window.setFramerateLimit(m_framerates[m_framerateIndex]);
			m_mainMenu.setTextSize(m_textSizes[newResolutionIndex]);
			m_mainMenu.setTarget(m_window);
		}
	);
	m_mainMenu.add(settingsPanel);
}

void MainMenuWindow::ProfileButtonPressed()
{
	auto userProfileExists = m_apiClient.GetUserProfile(m_apiClient.GetPlayerId());
	if (!userProfileExists)
	{
		Utils::CreateErrorPopupPanel(m_mainMenu, "Could not load profile");
		return;
	}
	auto panel = Utils::CreateProfilePanel(
		userProfileExists.value(),
		[this]
		{
			auto panel = m_mainMenu.get<tgui::Panel>("ProfilePanel");
			if (panel) m_mainMenu.remove(panel);
		}
	);
	m_mainMenu.add(panel, "ProfilePanel");
}

void MainMenuWindow::PlayPanelPressed()
{
	auto playPanel = PlayPanel::Create(
		m_mainMenu,
		m_apiClient,
		[this]() {},
		[this]() {},
		[this](ApiClient& api, const std::string& user)
		{
			auto lobbyId = api.CreateLobby(user);
			if (!lobbyId) return std::string{ "ERROR" };
			return lobbyId.value();
		}
	);
	m_mainMenu.add(playPanel);
}

void MainMenuWindow::CloseHostPanel()
{
	auto hostPanel = m_mainMenu.get<tgui::Panel>("HostPanel");
	if (hostPanel)
	{
		m_mainMenu.remove(hostPanel);
		m_apiClient.StopPingThread();
		m_apiClient.StopLobbyWatcher();
	}
}

void MainMenuWindow::ShutdownLobby()
{
	m_apiClient.StopLobbyWatcher();
	m_apiClient.StopPingThread();
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