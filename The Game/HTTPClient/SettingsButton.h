#pragma once
#include "IWindow.h"

class SettingsButton : IWindow
{
public:
	SettingsButton(sf::RenderWindow& window);

	void Draw() override;
	void HandleEvent(sf::Event event) override;

	void CloseButtonPressed();

	bool GetCloseState() const;
	bool GetApplyState() const;

	void SetCloseState();

private:
	void LoadWidgets() override;

private:
	tgui::Gui m_settingsButton;
	tgui::Panel::Ptr m_panel;

	bool m_closePressed{ false };
	bool m_applyPressed{ false };
};

