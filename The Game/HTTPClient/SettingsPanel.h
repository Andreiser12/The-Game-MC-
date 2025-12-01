#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include <vector>

class SettingsPanel
{
public:
	static tgui::Panel::Ptr Create(tgui::Gui& gui,
		std::function<void()> onClose,
		std::function<void(const sf::Vector2u& newResolution)> onApply);

private:

};

