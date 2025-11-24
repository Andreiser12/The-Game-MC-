#include "RegisterWindow.h"

RegisterWindow::RegisterWindow(sf::RenderWindow& window)
	: m_register(window)
{
	LoadWidgets();
}

void RegisterWindow::UpdateTextSize()
{
	const float windowHeight = m_register.getView().getRect().height;
	m_register.setTextSize(static_cast<unsigned int>(0.07f * windowHeight));
}

void RegisterWindow::Draw()
{
	m_register.draw();
}

void RegisterWindow::CheckRegister(tgui::EditBox::Ptr username)
{
	std::vector<std::string> dataBaseUsernames = { "user1", "name123", "guest" };
	if (username->getText() == "") return;

	if (username->getText() == "user1" ||
		username->getText() == "guest" ||
		username->getText() == "name123")
	{
		auto errorLabel = tgui::Label::create();
		errorLabel->setText("An account with this username already exists!");
		errorLabel->getRenderer()->setTextColor(tgui::Color::Red);
		auto coordErrorX = username->getPosition().x + username->getSize().x / 10000.f;
		auto coordErrorY = username->getPosition().y + username->getSize().y;
		errorLabel->setPosition(coordErrorX, coordErrorY);
		m_register.add(errorLabel);
		errorLabel->setTextSize(16);
		return;
	}
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
	m_register.add(editBoxUsername);
	editBoxUsername->onFocus([this, editBoxUsername] {
		editBoxUsername->setDefaultText("");
		});
	editBoxUsername->onUnfocus([this, editBoxUsername] {
		editBoxUsername->setDefaultText("Username");
		});

	auto registerButton = tgui::Button::create("Register"); registerButton->setSize({ "20%", "16.67%" });
	registerButton->setPosition({
		"usernameBox.w / 2 - registerButton.w / 2 + 30",
		"usernameBox.bottom + 75"
	});
	m_register.add(registerButton);
	registerButton->setTextSize(24);

	registerButton->onClick([this, editBoxUsername] {CheckRegister(editBoxUsername); });
}

bool RegisterWindow::GetRegisteredState() const
{
	return m_isRegistered;
}
