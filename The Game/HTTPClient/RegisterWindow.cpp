#include "RegisterWindow.h"

RegisterWindow::RegisterWindow(sf::RenderWindow& window, ApiClient& api)
	: m_register{window}, m_apiClient{api}
{
	m_register.setFont("assets/fonts/Cinzel-Regular.ttf");
	m_background = tgui::Picture::create("assets/images/mainMenuBackground.png");
	m_background->setSize({ "100%", "100%" });
	m_background->setPosition({ "0%", "0%" });
	m_register.add(m_background);
	LoadWidgets();
}

void RegisterWindow::UpdateTextSize()
{
	const float windowHeight{ m_register.getView().getRect().height };
	m_register.setTextSize(static_cast<unsigned>(0.07f * windowHeight));
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
		m_register.remove(m_register.get("errorPanel"));
		auto errorPanel = tgui::Panel::create({ "55%", "8%" });
		errorPanel->setPosition({ "50%", "65%" });
		errorPanel->setOrigin(0.5f, 0.5f);
		errorPanel->getRenderer()->setBackgroundColor(tgui::Color(30, 30, 30, 120));
		errorPanel->getRenderer()->setBorders(2);
		errorPanel->getRenderer()->setBorderColor(tgui::Color(200, 50, 50));
		errorPanel->getRenderer()->setRoundedBorderRadius(8);

		auto errorLabel = tgui::Label::create("An account with this username already exists!");
		errorLabel->getRenderer()->setTextColor(tgui::Color(255, 100, 100));
		errorLabel->setPosition({ "50%", "50%" });
		errorLabel->setOrigin(0.5f, 0.5f);
		errorLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		errorLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		errorLabel->getRenderer()->setTextSize(16);
		errorLabel->getRenderer()->setFont("assets/fonts/Arial.ttf");
		errorLabel->getRenderer()->setTextOutlineColor(tgui::Color(100, 20, 20));
		errorLabel->getRenderer()->setTextOutlineThickness(1.5f);

		errorPanel->add(errorLabel);
		m_register.add(errorPanel, "errorPanel");
		return;
	}
	m_apiClient.SetUsername(usernameEntered);
	m_apiClient.SetPlayerId(json["id"].i());
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
	editBoxUsername->setSize({ "65%", "12.5%" });
	editBoxUsername->setPosition({ "50%", "50%" });
	editBoxUsername->setOrigin(0.5f, 0.5f);
	editBoxUsername->setDefaultText("Username");
	editBoxUsername->getRenderer()->setTexture("assets/images/textBoxbackground2.png");
	m_register.add(editBoxUsername);
	editBoxUsername->onFocus([this, editBoxUsername] {
		editBoxUsername->setDefaultText("");
		});
	editBoxUsername->onUnfocus([this, editBoxUsername] {
		editBoxUsername->setDefaultText("Username");
		});

	auto registerButton = tgui::Button::create("Register");
	registerButton->getRenderer()->setTexture("assets/images/buttons.png");
	registerButton->setSize({ "20%", "15%" });
	registerButton->setPosition({"50%", "80%"});
	registerButton->setOrigin(0.5f, 0.5f);
	Utils::StyleButton(registerButton);
	m_register.add(registerButton);
	registerButton->onClick([this, editBoxUsername] {CheckRegister(editBoxUsername); });

	auto backButton = tgui::Button::create("Back");
	backButton->setSize({ "10%", "12.5%" });
	backButton->setPosition({ "3.5%", "5%" });
	backButton->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(backButton);
	m_register.add(backButton);
	backButton->onClick([this] { GoToLogin(); });
}

bool RegisterWindow::GetRegisteredState() const
{
	return m_isRegistered;
}
