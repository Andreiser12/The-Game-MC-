#include "LoginWindow.h"

LoginWindow::LoginWindow(sf::RenderWindow& window, ApiClient& api)
	: m_login{window}, m_apiClient{api}
{ 
	 
	m_background = tgui::Picture::create("assets/images/background.png");
	m_background->setSize({ "100%", "100%" });
	m_login.add(m_background);

	LoadWidgets();
} 

void LoginWindow::CheckLogin(tgui::EditBox::Ptr username)
{
	std::string usernameEntered = username->getText().toStdString();
	cpr::Response r = cpr::Post(
		cpr::Url{ "http://localhost:18080/login" },
		cpr::Body{ "{\"username\": \"" + usernameEntered + "\"}" },
		cpr::Header{ {"Content-Type", "application/json"} }
	);

	auto json = crow::json::load(r.text);

	if (!json || json["status"].s() == "fail")
	{
		auto errorLabel = tgui::Label::create();
		errorLabel->setText("No account with this username has been found!");
		errorLabel->getRenderer()->setTextColor(tgui::Color::Red);
		errorLabel->setPosition({ "50%", "62.5%" });
		errorLabel->setOrigin(0.5f, 0.5f);
		errorLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		errorLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		m_login.add(errorLabel);
		errorLabel->setTextSize(16);
		return;
	}
	m_apiClient.SetUsername(usernameEntered);
	m_isLoggedIn = true;
}

void LoginWindow::UpdateTextSize()
{ 
	const float windowHeight = m_login.getView().getRect().height;
	m_login.setTextSize(static_cast<unsigned int>(0.07f * windowHeight));
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
	UpdateTextSize();
	m_login.onViewChange([this] {UpdateTextSize(); });
	auto editBoxUsername = tgui::EditBox::create();
	editBoxUsername->setSize({ "66.67%", "12.5%" });
	editBoxUsername->setPosition({ "50%", "50%" });
	editBoxUsername->getRenderer()->setTexture("assets/images/textBoxbackground2.png");
	editBoxUsername->setOrigin(0.5f, 0.5f);
	editBoxUsername->setDefaultText("Username");
	m_login.add(editBoxUsername);
	editBoxUsername->onFocus([this, editBoxUsername] {
		editBoxUsername->setDefaultText("");
		});
	editBoxUsername->onUnfocus([this, editBoxUsername] {
		editBoxUsername->setDefaultText("Username");
		});

	auto loginButton = tgui::Button::create("Login");
	loginButton->setSize({ "20%", "16.67%" }); 
	auto coordX = editBoxUsername->getSize().x - editBoxUsername->getPosition().x;
	auto coordY = editBoxUsername->getSize().y*1.5f + editBoxUsername->getPosition().y;
	loginButton->setPosition({ coordX, coordY });
	m_login.add(loginButton);
	loginButton->setTextSize(24); 
	loginButton->getRenderer()->setTexture("assets/images/buttons.png");
	loginButton->onClick([this, editBoxUsername] { CheckLogin(editBoxUsername); });

	auto registerButton = tgui::Button::create("Register");
	registerButton->setSize({ "20%", "16.67%" });
	coordX = editBoxUsername->getPosition().x + loginButton->getSize().x*0.7f;
	coordY = editBoxUsername->getSize().y * 1.5f + editBoxUsername->getPosition().y;
	registerButton->setPosition({ coordX, coordY });
	registerButton->getRenderer()->setTexture("assets/images/buttons.png");
	m_login.add(registerButton); 
	registerButton->setTextSize(24);
	registerButton->onClick([this] {
		m_isRegisteredPressed = true;
		m_onLoginWindow = false;
		});
}

bool LoginWindow::GetLoggedState() const
{
	return m_isLoggedIn;
}

bool LoginWindow::GetRegisteredState() const
{
	return m_isRegisteredPressed;
}

bool LoginWindow::GetWindowState() const
{
	return m_onLoginWindow;
}
