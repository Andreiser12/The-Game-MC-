#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include <string>
#include <vector>

class LoginWindow
{
public:
	LoginWindow(sf::RenderWindow& window);

	void CheckLogin(tgui::EditBox::Ptr username);
	void UpdateTextSize();
	void Draw();
	void HandleEvent(sf::Event event);

	bool GetLoggedState() const;
	bool GetRegisteredState() const;

private:
	void LoadWidgets();
	
private:

	tgui::Gui m_login;
	bool m_isLoggedIn = false;
	bool m_isRegisteredPressed = false;
};

