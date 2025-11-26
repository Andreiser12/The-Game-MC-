#pragma once
#include "IWindow.h"

class ConnectionFailedWindow : IWindow
{
public:
	ConnectionFailedWindow(sf::RenderWindow& window);
	void HandleEvent(sf::Event event) override;
	void Draw() override;
	void UpdateTextSize() override;

private:
	void LoadWidgets() override;


private:
	tgui::Gui m_connectionFailed;
};

