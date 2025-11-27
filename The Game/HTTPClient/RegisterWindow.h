#pragma once
#include "IWindow.h"
#include <cpr/cpr.h>
#include <crow.h>
#include <vector>
#include <string>

class RegisterWindow : IWindow
{
public:
	RegisterWindow(sf::RenderWindow& window);

	void HandleEvent(sf::Event event) override;
	void Draw() override;
	void UpdateTextSize() override;
	void GoToLogin();
	
	void CheckRegister(tgui::EditBox::Ptr username);

	bool GetRegisteredState() const;

private:
	void LoadWidgets() override;

private:
	tgui::Gui m_register;
	tgui::Picture::Ptr m_background;
	bool m_isRegistered = false;
};

