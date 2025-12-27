#pragma once
#include "IWindow.h"
#include "ApiClient.h"
#include "Utils.h"

#include <cpr/cpr.h>
#include <crow.h>
#include <vector>
#include <string>

class RegisterWindow : IWindow
{
public:
	RegisterWindow(sf::RenderWindow& window, ApiClient& api);

	void HandleEvent(sf::Event event) override;
	void Draw() override;
	void UpdateTextSize();
	void GoToLogin();
	
	void CheckRegister(tgui::EditBox::Ptr username);

	bool GetRegisteredState() const;

private:
	void LoadWidgets() override;

private:
	tgui::Gui m_register;
	tgui::Picture::Ptr m_background;
	bool m_isRegistered = false;
	ApiClient& m_apiClient;
};

