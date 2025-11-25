#pragma once
#include "IWindow.h"

#include <string>
#include <vector>

class LoginWindow : IWindow
{
public:
	LoginWindow(sf::RenderWindow& window);

	void CheckLogin(tgui::EditBox::Ptr username);
	void CheckRegister();

	void UpdateTextSize() override;
	void Draw() override;
	void HandleEvent(sf::Event event) override;

	bool GetLoggedState() const;
	bool GetRegisteredState() const;
	bool GetWindowState() const override;

private:
	void LoadWidgets() override;
	
private:

	tgui::Gui m_login;
	bool m_isLoggedIn = false;
	bool m_isRegisteredPressed = false;
};

