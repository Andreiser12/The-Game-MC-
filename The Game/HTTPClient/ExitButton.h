#pragma once
#include "IWindow.h"

class ExitButton : IWindow
{
public:
	ExitButton(sf::RenderWindow& window);
	ExitButton() = default;

	void Draw() override;
	void HandleEvent(sf::Event event) override;
	void SetTarget(sf::RenderWindow& window);

	void NoPressed();
	void YesPressed();

	void SetNoState();

	bool GetNoState() const;
	bool GetYesState() const;
	tgui::Panel::Ptr GetPanel() const;

private:
	void LoadWidgets() override;

private:
	tgui::Gui m_exitButton;
	tgui::Panel::Ptr m_panel;

	bool m_noPressed{ false };
	bool m_yesPressed{ false };
};

