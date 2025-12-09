#pragma once
#include "IWindow.h"
#include "ApiClient.h"

#include <cpr/cpr.h>
#include <crow.h>
#include <string>
#include <vector>

class LoginWindow : IWindow
{
public:
	LoginWindow(sf::RenderWindow& window, ApiClient& api);

	void CheckLogin(tgui::EditBox::Ptr username);

	void UpdateTextSize();
	void Draw() override;
	void HandleEvent(sf::Event event) override;

	bool GetLoggedState() const;
	bool GetRegisteredState() const;
	bool GetWindowState() const override;

private:
	void LoadWidgets() override;
	
private:

	tgui::Gui m_login;
	tgui::Picture::Ptr m_background;
	bool m_isLoggedIn = false;
	bool m_isRegisteredPressed = false;
	ApiClient& m_apiClient;
};

