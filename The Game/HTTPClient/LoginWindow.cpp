module;
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>

module LoginWindow;

import IWindow;
import ApiClient;
import Utils;

LoginWindow::LoginWindow(sf::RenderWindow& window, ApiClient& api)
	: m_login{ window }, m_apiClient{ api }, m_clickSound{ m_clickBuffer }
{
	if (m_clickBuffer.loadFromFile("assets/sounds/press_button_sound.wav"))
	{
		m_clickSound.setBuffer(m_clickBuffer);
	}
	m_login.setFont("assets/fonts/Cinzel-Regular.ttf");
	m_background = tgui::Picture::create("assets/images/mainMenuBackground.png");
	m_background->setSize({ "100%", "100%" });
	m_background->setPosition("0%", "0%");
	m_login.add(m_background);
	LoadWidgets();
}

void LoginWindow::CheckLogin(const std::string& usernameEntered)
{
	if (LoginResult result = m_apiClient.Login(usernameEntered);  result != LoginResult::Success)
	{
		m_login.remove(m_login.get("errorPanel"));

		auto errorPanel = tgui::Panel::create({ "55%", "8%" });
		errorPanel->setPosition("50%", "62.5%");
		errorPanel->setOrigin(0.5f, 0.5f);
		errorPanel->getRenderer()->setBackgroundColor(tgui::Color(30, 30, 30, 120));
		errorPanel->getRenderer()->setBorders(2);
		errorPanel->getRenderer()->setBorderColor(tgui::Color(200, 50, 50));
		errorPanel->getRenderer()->setRoundedBorderRadius(8);

		auto errorLabel = tgui::Label::create();
		switch (result)
		{
		case LoginResult::UserNotFound:
			errorLabel->setText("No account with this username has been found!");
			break;
		case LoginResult::AlreadyLoggedIn:
			errorLabel->setText("You're already logged in!");
			break;
		case LoginResult::NetworkError:
			errorLabel->setText("Server error. Please try again!");
			break;
		default:
			break;
		}
		errorLabel->getRenderer()->setTextColor(tgui::Color(255, 100, 100));
		errorLabel->setPosition("50%", "50%");
		errorLabel->setOrigin(0.5f, 0.5f);
		errorLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		errorLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		errorLabel->getRenderer()->setTextSize(16);
		errorLabel->getRenderer()->setFont("assets/fonts/Arial.ttf");
		errorLabel->getRenderer()->setTextOutlineColor(tgui::Color(100, 20, 20));
		errorLabel->getRenderer()->setTextOutlineThickness(1.5f);

		errorPanel->add(errorLabel);
		m_login.add(errorPanel, "errorPanel");
		return;
	}
	m_isLoggedIn = true;
}

void LoginWindow::Draw()
{
	m_login.draw();
}

void LoginWindow::HandleEvent(sf::Event event)
{
	m_login.handleEvent(event);
}

void LoginWindow::LoadWidgets()

{
	auto editBoxUsername = tgui::EditBox::create();
	editBoxUsername->getRenderer()->setFont("assets/fonts/Sanchez-Regular.ttf");
	editBoxUsername->setSize("65%", "12.5%");
	editBoxUsername->setPosition("50%", "50%");
	editBoxUsername->getRenderer()->setTexture("assets/images/textBoxbackground.png");
	editBoxUsername->setOrigin(0.5f, 0.5f);
	editBoxUsername->setTextSize(36);
	editBoxUsername->setDefaultText("Username");
	m_login.add(editBoxUsername);
	editBoxUsername->onFocus([this, editBoxUsername] {
		editBoxUsername->setDefaultText("");
		});
	editBoxUsername->onUnfocus([this, editBoxUsername] {
		editBoxUsername->setDefaultText("Username");
		});
	editBoxUsername->onReturnKeyPress([this, editBoxUsername]
		{ CheckLogin(editBoxUsername->getText().toStdString()); });

	auto loginButton = tgui::Button::create("Login");
	loginButton->getRenderer()->setTexture("assets/images/buttons.png");
	loginButton->setSize("20%", "15%");
	loginButton->setPosition("18%", "71%");
	Utils::StyleButton(loginButton);
	m_login.add(loginButton);
	loginButton->onClick([this, editBoxUsername] {
		m_clickSound.play();
		CheckLogin(editBoxUsername->getText().toStdString());
		});

	auto registerButton = tgui::Button::create("Register");
	registerButton->getRenderer()->setTexture("assets/images/buttons.png");
	registerButton->setSize("20%", "15%");
	registerButton->setPosition("62.5%", "71%");
	Utils::StyleButton(registerButton);
	m_login.add(registerButton);
	registerButton->onClick([this] {
		m_clickSound.play();
		m_isRegisteredPressed = true;
		m_onLoginWindow = false;
		});
}

bool LoginWindow::GetLoggedState() const noexcept
{
	return m_isLoggedIn;
}

bool LoginWindow::GetRegisteredState() const noexcept
{
	return m_isRegisteredPressed;
}

bool LoginWindow::GetWindowState() const noexcept
{
	return m_onLoginWindow;
}
