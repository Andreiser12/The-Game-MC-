#include "RegisterWindow.h"

RegisterWindow::RegisterWindow(sf::RenderWindow& window, ApiClient& api)
	: m_register{window}, m_apiClient{api}
{
	m_background = tgui::Picture::create("assets/images/background.png");
	m_background->setSize({ "100%", "100%" });
	m_register.add(m_background);

	LoadWidgets();
}

void RegisterWindow::UpdateTextSize()
{
	const float windowHeight = m_register.getView().getRect().height;
	m_register.setTextSize(static_cast<unsigned int>(0.07f * windowHeight));
}

void RegisterWindow::GoToLogin()
{
	m_onLoginWindow = true;
}

void RegisterWindow::Draw()
{
	m_register.draw();
}

void RegisterWindow::CheckRegister(tgui::EditBox::Ptr username)
{
	std::string usernameEntered = username->getText().toStdString();
	cpr::Response r = cpr::Post(
		cpr::Url{ "http://localhost:18080/register" },
		cpr::Body{ "{\"username\": \"" + usernameEntered + "\"}" },
		cpr::Header{ {"Content-Type", "application/json"} }
	);
	auto json = crow::json::load(r.text);
	if (!json || json["status"].s() == "fail")
	{
		auto errorLabel = tgui::Label::create();
		errorLabel->setText("An account with this username already exists!");
		errorLabel->getRenderer()->setTextColor(tgui::Color::Red);
		errorLabel->setPosition({ "50%", "65%" });
		errorLabel->setOrigin(0.5f, 0.5f);
		errorLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		errorLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		m_register.add(errorLabel);
		errorLabel->setTextSize(16);
		return;
	}
	m_apiClient.SetUsername(usernameEntered);
	m_isRegistered = true;
}

void RegisterWindow::HandleEvent(sf::Event event)
{
	m_register.handleEvent(event);
}

void RegisterWindow::LoadWidgets()
{
	UpdateTextSize();
	m_register.onViewChange([this] {UpdateTextSize(); });
	auto editBoxUsername = tgui::EditBox::create();
	editBoxUsername->setSize({ "66.67%", "12.5%" });
	editBoxUsername->setPosition({ "50%", "50%" });
	editBoxUsername->setOrigin(0.5f, 0.5f);
	editBoxUsername->setDefaultText("Username");
	editBoxUsername->setWidgetName("usernameBox");
	editBoxUsername->getRenderer()->setTexture("assets/images/textBoxbackground2.png");
	m_register.add(editBoxUsername);
	editBoxUsername->onFocus([this, editBoxUsername] {
		editBoxUsername->setDefaultText("");
		});
	editBoxUsername->onUnfocus([this, editBoxUsername] {
		editBoxUsername->setDefaultText("Username");
		});

	auto registerButton = tgui::Button::create("Register");
	registerButton->setSize({ "20%", "16.67%" });
	registerButton->setPosition({
		"usernameBox.w / 2 - registerButton.w / 2 + 30",
		"usernameBox.bottom + 75"
		});
	registerButton->getRenderer()->setTexture("assets/images/buttons.png");
	m_register.add(registerButton);
	registerButton->setTextSize(24);

	registerButton->onClick([this, editBoxUsername] {CheckRegister(editBoxUsername); });

	auto backButton = tgui::Button::create("Back");
	backButton->setSize({ "10%", "12.5%" });
	backButton->setPosition({ "3.5%", "5%" });
	backButton->getRenderer()->setTexture("assets/images/buttons.png");
	m_register.add(backButton);
	backButton->setTextSize(20);
	backButton->onClick([this] { GoToLogin(); });
}

bool RegisterWindow::GetRegisteredState() const
{
	return m_isRegistered;
}
