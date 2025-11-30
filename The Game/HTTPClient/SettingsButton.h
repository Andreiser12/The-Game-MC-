#pragma once
#include "IWindow.h"
#include <vector>

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

	std::vector<sf::Vector2u> m_resolutions = 
	{ {640, 480},
		{800, 600},
		{1024, 768},
		{1280, 720},
		{1280, 800},
		{1366, 768},
		{1440, 900},
		{1680, 1050},
		{1920, 1080},
		{1920, 1200},
		{2560, 1440}
	};
	unsigned m_resolutionIndex = 8u;
};

