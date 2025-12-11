#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#include "ApiClient.h"

class GamePanel
{
public:
	static tgui::Group::Ptr Create(tgui::Gui& gui,
		ApiClient& apiClient);

private:
};

