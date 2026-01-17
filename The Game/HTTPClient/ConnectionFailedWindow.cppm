module;
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <TGUI/Widgets/Label.hpp>

export module ConnectionFailedWindow;

import IWindow;

export class ConnectionFailedWindow : public IWindow
{
public:
	explicit ConnectionFailedWindow(sf::RenderWindow& window);

	ConnectionFailedWindow(const ConnectionFailedWindow&) = delete;
	ConnectionFailedWindow& operator=(const ConnectionFailedWindow&) = delete;
	ConnectionFailedWindow(ConnectionFailedWindow&&) noexcept = default;
	ConnectionFailedWindow& operator=(ConnectionFailedWindow&&) noexcept = default;

	void HandleEvent(sf::Event event) override;
	void Draw() override;

private:
	void LoadWidgets() override;

private:
	tgui::Gui m_connectionFailed;
};

