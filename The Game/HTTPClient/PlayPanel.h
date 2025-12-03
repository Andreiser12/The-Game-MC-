#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include "JoinPanel.h"

class PlayPanel
{
public:
	static tgui::Panel::Ptr Create(tgui::Gui& gui,
		std::function<void()> onClose,
		std::function<void()> onJoin,
		std::function<void()> onHost);

private:

};

