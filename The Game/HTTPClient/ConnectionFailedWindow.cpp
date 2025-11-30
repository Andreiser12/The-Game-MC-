#include "ConnectionFailedWindow.h"

ConnectionFailedWindow::ConnectionFailedWindow(sf::RenderWindow& window) : m_connectionFailed{ window }
{
	LoadWidgets();
}

void ConnectionFailedWindow::HandleEvent(sf::Event event)
{
	m_connectionFailed.handleEvent(event);
}

void ConnectionFailedWindow::Draw()
{
	m_connectionFailed.draw();
}

void ConnectionFailedWindow::LoadWidgets()
{
	auto errorLabel = tgui::Label::create();
	errorLabel->setSize({ "66.67%", "12.5%"});
	errorLabel->setPosition({"50%", "50%"});
	errorLabel->setOrigin(0.5f, 0.5f);
	errorLabel->setText("Couldn't connect to the server!");
	errorLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	errorLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	errorLabel->getRenderer()->setTextColor(tgui::Color::Red);
	m_connectionFailed.add(errorLabel);
	errorLabel->setTextSize(24);
}
