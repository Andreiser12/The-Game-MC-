#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <functional>
#include <vector>
#include "UserProfile.h"

class Utils
{
public:
	static tgui::String TruncateText(const tgui::String& text, uint8_t maxChars);

	static tgui::Panel::Ptr CreateSettingsPanel(tgui::Gui& gui,
		unsigned resolutionIndex,
		bool isFullscreen,
		unsigned framerateIndex,
		std::function<void()> onClose,
		std::function<void(unsigned res, bool fullscreen, unsigned framerate)> onApply);

	static tgui::Panel::Ptr	CreateProfilePanel(
		const UserProfile& profile,
		std::function<void()> onClose);

	static tgui::Panel::Ptr	CreateExitPanel(
		tgui::Gui& gui,
		std::function<void()> onYes,
		std::function<void()> onNo);

	static tgui::Panel::Ptr CreateErrorPopupPanel(tgui::Gui& gui,
		const std::string& message, std::function<void()> onClose = nullptr);

	static void StyleButton(const tgui::Button::Ptr& button);

private:
};

