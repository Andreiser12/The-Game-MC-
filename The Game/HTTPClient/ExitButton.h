#pragma once
#include "IWindow.h"

class ExitButton : IWindow
{
public:
	ExitButton(sf::RenderWindow& window);
	ExitButton() = default;

	void Draw() override;
	void HandleEvent(sf::Event event) override;
	void UpdateTextSize() override;

private:
	void LoadWidgets() override;

private:
	tgui::Gui m_exitButton;
};

