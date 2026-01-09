#pragma once
#include "SFML/Graphics.hpp"

class IWindow
{
protected:
	virtual void HandleEvent(sf::Event event) = 0;
	virtual void Draw() = 0;
	virtual void LoadWidgets() = 0;
	virtual bool GetWindowState() const
	{
		return m_onLoginWindow;
	}

protected:
	static bool m_onLoginWindow;
};
