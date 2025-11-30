#include "MainMenuWindow.h"

constexpr static unsigned int K_OFFSET_BUTTONS{ 100u };
constexpr static unsigned int K_TEXT_SIZE{ 24u };

MainMenuWindow::MainMenuWindow(sf::RenderWindow& window) :
	m_window(window),
	m_mainMenu(window),
	m_exitLayout(window),
	m_settingsLayout(window)
{
	m_background = tgui::Picture::create("assets/images/background.png");
	m_background->setSize({ "100%", "100%" });
	m_mainMenu.add(m_background);

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
	playButton->getRenderer()->setOpacity(0.7);
	playButton->getRenderer()->setTexture("assets/images/buttons.png");

	m_mainMenu.add(playButton);
	playButton->setTextSize(K_TEXT_SIZE);


	profileButton->setSize({ "16.67%", "5%" });
	profileButton->setPosition({ "50%",
		playButton->getPosition().y + K_OFFSET_BUTTONS
		});
	profileButton->setOrigin(0.5f, 0.5f);
	profileButton->setText("Profile");
	profileButton->getRenderer()->setOpacity(0.7);
	profileButton->getRenderer()->setTexture("assets/images/buttons.png");

	m_mainMenu.add(profileButton);
	profileButton->setTextSize(K_TEXT_SIZE);


	settingsButton->setSize({ "16.67%", "5%" });
	settingsButton->setPosition({ "50%",
		profileButton->getPosition().y + K_OFFSET_BUTTONS });
	settingsButton->setOrigin(0.5f, 0.5f);
	settingsButton->setText("Settings");
	settingsButton->getRenderer()->setOpacity(0.7);
	settingsButton->getRenderer()->setTexture("assets/images/buttons.png");

	m_mainMenu.add(settingsButton);
	settingsButton->setTextSize(K_TEXT_SIZE);
	settingsButton->onClick([this] { SettingsButtonPressed(); });

	exitButton->setSize({ "16.67%", "5%" });
	exitButton->setPosition({ "50%",
		settingsButton->getPosition().y + K_OFFSET_BUTTONS });
	exitButton->setOrigin(0.5f, 0.5f);
	exitButton->setText("Exit");
	exitButton->getRenderer()->setOpacity(0.7);
	exitButton->getRenderer()->setBackgroundColor(tgui::Color::Yellow);
	exitButton->getRenderer()->setTexture("assets/images/buttons.png");

	m_mainMenu.add(exitButton);
	exitButton->setTextSize(K_TEXT_SIZE);
	exitButton->onClick([this] {ExitButtonPressed(); });
}

void MainMenuWindow::Draw()
{
	m_mainMenu.draw();

	switch (m_uiState)
	{
	case UiState::ExitMenu:
		m_exitLayout.Draw();
		break;

	case UiState::SettingsMenu:
		m_settingsLayout.Draw();
		break;

	default:
		break;
	}
}

void MainMenuWindow::ExitButtonPressed()
{
	if (m_uiState == UiState::MainMenu)
	{
		m_uiState = UiState::ExitMenu;
	}
}

void MainMenuWindow::SettingsButtonPressed()
{
	if (m_uiState == UiState::MainMenu)
	{
		m_uiState = UiState::SettingsMenu;
	}
}

void MainMenuWindow::HandleEvent(sf::Event event)
{
	if (m_uiState == UiState::MainMenu)
	{
		m_mainMenu.handleEvent(event);
	}

	if (m_uiState == UiState::ExitMenu)
	{
		m_exitLayout.HandleEvent(event);

		if (m_exitLayout.GetNoState())
		{
			m_uiState = UiState::MainMenu;
			m_exitLayout.SetNoState();
		}

		if (m_exitLayout.GetYesState())
		{
			m_window.close();
		}
	}

	if (m_uiState == UiState::SettingsMenu)
	{
		m_settingsLayout.HandleEvent(event);

		if (m_settingsLayout.GetCloseState())
		{
			m_uiState = UiState::MainMenu;
			m_settingsLayout.SetCloseState();
		}
		if (m_settingsLayout.GetApplyState())
		{
			auto newRes = m_settingsLayout.GetCurrentResolution();
			m_window.create(sf::VideoMode(newRes), "The Game - 1.0.0", sf::State::Fullscreen);
			m_mainMenu.setTarget(m_window);
			m_exitLayout.SetTarget(m_window);
			m_settingsLayout.SetTarget(m_window);
			m_settingsLayout.SetApplyState();
		}
	}
}