#include "MainMenuWindow.h"
#include <iostream>
#include <fstream>

constexpr static unsigned int K_OFFSET_BUTTONS{ 100u };
constexpr static unsigned int K_TEXT_SIZE{ 24u };

MainMenuWindow::MainMenuWindow(sf::RenderWindow& window) :
	m_window(window),
	m_mainMenu(window),
	m_exitLayout(window)
{
	m_mainMenu.setTarget(window);

	// BACKGROUND
	m_background = tgui::Picture::create("assets/images/MainMenuBackground.png");
	m_background->setSize({ "100%", "100%" }); // fullscreen
	m_mainMenu.add(m_background);

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
	playButton->getRenderer()->setOpacity(0.7);
	playButton->getRenderer()->setTexture("assets/images/ceva2.png");

	m_mainMenu.add(playButton);
	playButton->setTextSize(K_TEXT_SIZE);
	

	profileButton->setSize({ "16.67%", "5%" });
	profileButton->setPosition({ "50%",
		playButton->getPosition().y + K_OFFSET_BUTTONS
		});
	profileButton->setOrigin(0.5f, 0.5f);
	profileButton->setText("Profile");
	profileButton->getRenderer()->setOpacity(0.5);
	profileButton->getRenderer()->setTexture("assets/images/ceva2.png");

	m_mainMenu.add(profileButton);
	profileButton->setTextSize(K_TEXT_SIZE);


	settingsButton->setSize({ "16.67%", "5%" });
	settingsButton->setPosition({ "50%",
		profileButton->getPosition().y + K_OFFSET_BUTTONS });
	settingsButton->setOrigin(0.5f, 0.5f);
	settingsButton->setText("Settings");
	settingsButton->getRenderer()->setOpacity(0.5);
	settingsButton->getRenderer()->setTexture("assets/images/ceva2.png");

	m_mainMenu.add(settingsButton);
	settingsButton->setTextSize(K_TEXT_SIZE);


	exitButton->setSize({ "16.67%", "5%" });
	exitButton->setPosition({ "50%",
		settingsButton->getPosition().y + K_OFFSET_BUTTONS });
	exitButton->setOrigin(0.5f, 0.5f);
	exitButton->setText("Exit");
	exitButton->getRenderer()->setOpacity(0.5);
	exitButton->getRenderer()->setTexture("assets/images/ceva2.png");

	m_mainMenu.add(exitButton);
	exitButton->setTextSize(K_TEXT_SIZE);
	exitButton->onClick([this] {ExitButtonPressed (); });
}

void MainMenuWindow::Draw()
{
	m_mainMenu.draw();
	if (m_showExitMenu)
	{
		m_exitLayout.Draw();
	}
}

void MainMenuWindow::ExitButtonPressed()
{
	m_showExitMenu = true;
}

void MainMenuWindow::HandleEvent(sf::Event event)
{
	m_mainMenu.handleEvent(event);

	if (m_showExitMenu)
	{
		m_exitLayout.HandleEvent(event);

		if (m_exitLayout.GetNoState())
		{
			m_exitLayout.SetNoState();
			m_showExitMenu = false;
		}

		if (m_exitLayout.GetYesState())
		{
			m_exitLayout.SetYesState();
			m_window.close();
		}
	}
}

