#include "MainMenuWindow.h"

constexpr static unsigned int K_OFFSET_BUTTONS{ 100u };
constexpr static unsigned int K_TEXT_SIZE{ 24u };

MainMenuWindow::MainMenuWindow(sf::RenderWindow& window) :
	m_mainMenu(window)
{
	LoadWidgets();
}

void MainMenuWindow::LoadWidgets()
{
	auto playButton = tgui::Button::create();
	auto profileButton = tgui::Button::create();
	auto settingsButton = tgui::Button::create();
	auto exitButton = tgui::Button::create();

	playButton->setSize({"16.67%", "5%"});
	playButton->setPosition({ "50%", "42.5%" });
	playButton->setOrigin(0.5f, 0.5f);
	playButton->setText("Play");
	m_mainMenu.add(playButton);
	playButton->setTextSize(K_TEXT_SIZE);

	profileButton->setSize({ "16.67%", "5%" });
	profileButton->setPosition({ "50%",
		playButton->getPosition().y + K_OFFSET_BUTTONS
		});
	profileButton->setOrigin(0.5f, 0.5f);
	profileButton->setText("Profile");
	m_mainMenu.add(profileButton);
	profileButton->setTextSize(K_TEXT_SIZE);

	settingsButton->setSize({ "16.67%", "5%" });
	settingsButton->setPosition({ "50%",
		profileButton->getPosition().y + K_OFFSET_BUTTONS });
	settingsButton->setOrigin(0.5f, 0.5f);
	settingsButton->setText("Settings");
	m_mainMenu.add(settingsButton);
	settingsButton->setTextSize(K_TEXT_SIZE);

	exitButton->setSize({ "16.67%", "5%" });
	exitButton->setPosition({ "50%",
		settingsButton->getPosition().y + K_OFFSET_BUTTONS });
	exitButton->setOrigin(0.5f, 0.5f);
	exitButton->setText("Exit");
	m_mainMenu.add(exitButton);
	exitButton->setTextSize(K_TEXT_SIZE);
}

void MainMenuWindow::Draw()
{
	m_mainMenu.draw();
}

void MainMenuWindow::HandleEvent(sf::Event event)
{
	m_mainMenu.handleEvent(event);
}

