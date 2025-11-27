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
	LoadWidgets();
}

void MainMenuWindow::LoadWidgets()
{
	auto playButton = tgui::BitmapButton::create();
	auto profileButton = tgui::BitmapButton::create();
	auto settingsButton = tgui::BitmapButton::create();
	auto exitButton = tgui::BitmapButton::create();

	playButton->setSize({"16.67%", "5%"});
	playButton->setPosition({ "50%", "42.5%" });
	playButton->setOrigin(0.5f, 0.5f);
	playButton->setText("Play");
	playButton->getRenderer()->setBackgroundColor(tgui::Color::Yellow);
	playButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	playButton->getRenderer()->setBackgroundColorDown(tgui::Color::Yellow);
	playButton->getRenderer()->setOpacity(0.5);
	//playButton->getRenderer()->setTexture("ceva2.png");
	m_mainMenu.add(playButton);
	playButton->setTextSize(K_TEXT_SIZE);
	

	profileButton->setSize({ "16.67%", "5%" });
	profileButton->setPosition({ "50%",
		playButton->getPosition().y + K_OFFSET_BUTTONS
		});
	profileButton->setOrigin(0.5f, 0.5f);
	profileButton->setText("Profile");
	profileButton->getRenderer()->setBackgroundColor(tgui::Color::Yellow);
	profileButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	profileButton->getRenderer()->setBackgroundColorDown(tgui::Color::Yellow);
	profileButton->getRenderer()->setOpacity(0.5);
	m_mainMenu.add(profileButton);
	profileButton->setTextSize(K_TEXT_SIZE);

	settingsButton->setSize({ "16.67%", "5%" });
	settingsButton->setPosition({ "50%",
		profileButton->getPosition().y + K_OFFSET_BUTTONS });
	settingsButton->setOrigin(0.5f, 0.5f);
	settingsButton->setText("Settings");
	settingsButton->getRenderer()->setBackgroundColor(tgui::Color::Yellow);
	settingsButton->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
	settingsButton->getRenderer()->setBackgroundColorDown(tgui::Color::Yellow);
	settingsButton->getRenderer()->setOpacity(0.5);
	m_mainMenu.add(settingsButton);
	settingsButton->setTextSize(K_TEXT_SIZE);

	exitButton->setSize({ "16.67%", "5%" });
	exitButton->setPosition({ "50%",
		settingsButton->getPosition().y + K_OFFSET_BUTTONS });
	exitButton->setOrigin(0.5f, 0.5f);
	exitButton->setText("Exit");
	exitButton->getRenderer()->setBackgroundColor(tgui::Color::Red);
	exitButton->getRenderer()->setBackgroundColorHover(tgui::Color::Red);
	exitButton->getRenderer()->setBackgroundColorDown(tgui::Color::Red);
	exitButton->getRenderer()->setOpacity(0.5);
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

