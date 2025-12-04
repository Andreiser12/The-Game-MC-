#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

class HostPanel
{
public:
	static tgui::Panel::Ptr Create(tgui::Gui& gui,
		std::function<void()> onLeave,
		std::function<void()> onStart);

private:

};

