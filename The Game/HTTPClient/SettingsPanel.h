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
		unsigned resolutionIndex,
		std::function<void()> onClose,
		std::function<void(unsigned res)> onApply);

private:

};

