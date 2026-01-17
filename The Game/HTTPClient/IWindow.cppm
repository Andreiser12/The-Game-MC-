module;
#include "SFML/Graphics.hpp"

export module IWindow;

export class IWindow
{
protected:
	virtual ~IWindow() = default;

	virtual void HandleEvent(sf::Event event) = 0;
	virtual void Draw() = 0;
	virtual void LoadWidgets() = 0;
	virtual bool GetWindowState() const noexcept { return m_onLoginWindow; }

protected:
	static inline bool m_onLoginWindow{ true };
};