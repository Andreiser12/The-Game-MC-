#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include <string>

class LoginWindow
{
public:
	LoginWindow(sf::RenderWindow& window);

	void Login(tgui::EditBox::Ptr username);
	void UpdateTextSize();
	void Draw();

private:
	void LoadWidgets();
	
private:

	tgui::Gui m_login;
};

