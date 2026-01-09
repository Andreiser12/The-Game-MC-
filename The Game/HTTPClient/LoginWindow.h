#pragma once
#include "IWindow.h"
#include "ApiClient.h"
#include "Utils.h"

#include <cpr/cpr.h>
#include <crow.h>

class LoginWindow : IWindow
{
public:
	LoginWindow(sf::RenderWindow& window, ApiClient& api);

	void CheckLogin(const std::string& usernameEntered);

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
	bool m_isLoggedIn{ false };
	bool m_isRegisteredPressed{ false };
	ApiClient& m_apiClient;
};

