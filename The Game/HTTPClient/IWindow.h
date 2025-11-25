#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

class IWindow
{
protected:
	virtual void HandleEvent(sf::Event event) = 0;
	virtual void Draw() = 0;
	virtual void UpdateTextSize() = 0;
	virtual void LoadWidgets() = 0;
	virtual bool GetWindowState() const
	{
		return m_onLoginWindow;
	}

protected:
	static bool m_onLoginWindow;
};
