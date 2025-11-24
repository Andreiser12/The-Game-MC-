#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include <vector>
#include <string>

class RegisterWindow
{
public:
	RegisterWindow(sf::RenderWindow& window);

	void HandleEvent(sf::Event event);
	void Draw();
	void UpdateTextSize();
	
	void CheckRegister(tgui::EditBox::Ptr username);

	bool GetRegisteredState() const;

private:
	void LoadWidgets();

private:
	tgui::Gui m_register;
	bool m_isRegistered = false;
};

