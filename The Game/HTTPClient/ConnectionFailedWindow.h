#pragma once
#include "IWindow.h"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

class ConnectionFailedWindow : public IWindow
{
public:
	ConnectionFailedWindow(sf::RenderWindow& window);
	void HandleEvent(sf::Event event) override;
	void Draw() override;

private:
	void LoadWidgets() override;

private:
	tgui::Gui m_connectionFailed;
};

