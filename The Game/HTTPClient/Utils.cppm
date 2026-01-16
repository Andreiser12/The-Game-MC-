module;
#define NOMINMAX
#include <string>
#include <vector>
#include <functional>
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <SFML/Audio.hpp>

export module Utils;

import UserProfile;

export class Utils
{
public:
	static tgui::String TruncateText(const tgui::String& text, uint8_t maxChars);

	static tgui::Panel::Ptr CreateSettingsPanel(tgui::Gui& gui,
		unsigned resolutionIndex,
		bool isFullscreen,
		unsigned framerateIndex,
		float masterVolume,
		float buttonsVolume,
		float backgroundVolume,
		sf::Sound& btnSound,
		std::function<void()> onClose,
		std::function<void(unsigned res, bool fullscreen, unsigned framerate,
			float masterVol, float btnVol, float bgVol)> onApply);

	static tgui::Panel::Ptr	CreateProfilePanel(
		const UserProfile& profile,
		sf::Sound& btnSound,
		std::function<void()> onClose);

	static tgui::Panel::Ptr	CreateExitPanel(
		tgui::Gui& gui,
		sf::Sound& btnSound,
		std::function<void()> onYes,
		std::function<void()> onNo);

	static tgui::Panel::Ptr CreateErrorPopupPanel(
		tgui::Gui& gui,
		const std::string& message,
		sf::Sound& btnSound,
		std::function<void()> onClose = nullptr);

	static void StyleButton(const tgui::Button::Ptr& button);
	static void StyleSlider(const tgui::Slider::Ptr& slider);
};

