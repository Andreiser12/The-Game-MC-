#include "MainMenuWindow.h"

constexpr static uint8_t K_TEXT_SIZE{ 24u };

MainMenuWindow::MainMenuWindow(sf::RenderWindow& window, ApiClient& api) :
	m_window{ window },
	m_mainMenu{ window },
	m_apiClient{api}
{
	m_background = tgui::Picture::create("assets/images/mainMenuBackground.png");
	m_background->setSize({ "100%", "100%" });
	m_background->setPosition({ "0%", "0%" });
	m_mainMenu.add(m_background);

	auto gameTitle = tgui::Picture::create("assets/images/gameTitle.png");
	gameTitle->setSize({ "50%", "50%" });
	gameTitle->setPosition({ "50%", "15%" });
	gameTitle->setOrigin(0.5f, 0.1f);
	m_mainMenu.add(gameTitle);

	/*const int frameCount = 60;
	m_bgFrames.reserve(frameCount);

	for (int i = 0; i < frameCount; i++)
	{
		sf::Texture tempTex;

		std::string filename = "assets/images/TheGameLevelSequence-ezgif.com-speed" + std::to_string(i) + ".png";

		if (!tempTex.loadFromFile(filename))
		{
			std::cerr << "Eroare la incarcarea imaginii: "  << filename << std::endl;
			continue;
		}
		m_bgFrames.push_back(std::move(tempTex));
	}

	if (!m_bgFrames.empty())
	{
		m_bgSprite.emplace();
		m_bgSprite->setTexture(m_bgFrames[0], true);
		m_bgSprite->setPosition({ 0.f, 0.f });

		auto texSize = m_bgFrames[0].getSize();
		auto winSize = m_window.getSize();

		sf::Vector2f scale{
			static_cast<float>(winSize.x) / texSize.x,
			static_cast<float>(winSize.y) / texSize.y
		};

		m_bgSprite->setScale(scale);
	}*/

	LoadWidgets();
}

void MainMenuWindow::LoadWidgets()
{
	auto playButton = tgui::Button::create();
	auto profileButton = tgui::Button::create();
	auto settingsButton = tgui::Button::create();
	auto exitButton = tgui::Button::create();

	playButton->setSize({ "16.67%", "5%" });
	playButton->setPosition({ "50%", "42.5%" });
	playButton->setOrigin(0.5f, 0.5f);
	playButton->setText("Play");
	playButton->getRenderer()->setOpacity(0.7f);
	playButton->getRenderer()->setTexture("assets/images/buttons.png");

	m_mainMenu.add(playButton);
	playButton->onClick([this] {PlayPanelPressed(); });
	playButton->setTextSize(K_TEXT_SIZE);

	profileButton->setSize({ "16.67%", "5%" });
	profileButton->setPosition({ "50%", "52.5%" });
	profileButton->setOrigin(0.5f, 0.5f);
	profileButton->setText("Profile");
	profileButton->getRenderer()->setOpacity(0.7f);
	profileButton->getRenderer()->setTexture("assets/images/buttons.png");

	m_mainMenu.add(profileButton);
	profileButton->setTextSize(K_TEXT_SIZE);

	settingsButton->setSize({ "16.67%", "5%" });
	settingsButton->setPosition({ "50%", "62.5%" });
	settingsButton->setOrigin(0.5f, 0.5f);
	settingsButton->setText("Settings");
	settingsButton->getRenderer()->setTexture("assets/images/buttons.png");

	m_mainMenu.add(settingsButton);
	settingsButton->setTextSize(K_TEXT_SIZE);
	settingsButton->onClick([this] { SettingsButtonPressed(); });

	exitButton->setTextSize(K_TEXT_SIZE);
	exitButton->setSize({ "16.67%", "5%" });
	exitButton->setPosition({ "50%", "72.5%" });
	exitButton->setOrigin(0.5f, 0.5f);
	exitButton->setText("Exit");
	exitButton->getRenderer()->setTexture("assets/images/buttons.png");

	m_mainMenu.add(exitButton);
	exitButton->onClick([this] {ExitButtonPressed(); });
}

void MainMenuWindow::Draw()
{
	/*if (m_bgSprite && m_bgClock.getElapsedTime().asSeconds() >= m_bgFrameDuration)
	{
		m_bgClock.restart();
		m_bgCurrentFrame = (m_bgCurrentFrame + 1) % m_bgFrames.size();
		m_bgSprite->setTexture(m_bgFrames[m_bgCurrentFrame]);
	}
	if (m_bgSprite)
		m_window.draw(*m_bgSprite);*/
	m_mainMenu.draw();
}

void MainMenuWindow::Update()
{
	if (m_gamePanel)
	{
		m_gamePanel->Update();
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
	auto lobbyState = m_apiClient.GetLobbyState(m_apiClient.GetLobbyId());
	if (!lobbyState) return;

	if (lobbyState.value().m_status == "GAME_STARTED" && !m_gamePanel)
	{
		m_gamePanel = std::make_shared<GamePanel>(m_mainMenu, m_apiClient);
		m_gamePanel->OnGameStarted();
		m_gameStartedLocally = true;
	}
}

void MainMenuWindow::ExitButtonPressed()
{
	auto exitPanel = Utils::CreateExitPanel(
		m_mainMenu,
		[this]() {
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
	m_apiClient.SetPlayerId(0);
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