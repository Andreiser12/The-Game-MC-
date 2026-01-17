module;
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
import ApiClient;

export namespace Utils
{
	tgui::String TruncateText(const tgui::String& text, uint8_t maxChars);

	tgui::Panel::Ptr CreateSettingsPanel(tgui::Gui& gui,
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

	tgui::Panel::Ptr CreateProfilePanel(
		const UserProfile& profile,
		sf::Sound& btnSound,
		std::function<void()> onClose);

	tgui::Panel::Ptr CreateExitPanel(
		tgui::Gui& gui,
		sf::Sound& btnSound,
		std::function<void()> onYes,
		std::function<void()> onNo);

	tgui::Panel::Ptr CreateErrorPopupPanel(
		tgui::Gui& gui,
		const std::string& message,
		sf::Sound& btnSound,
		std::function<void()> onClose = nullptr);

	tgui::Panel::Ptr CreatePlayPanel(tgui::Gui& gui,
		ApiClient& apiClient,
		sf::Sound& btnSound,
		std::function<void()>&& onClose,
		std::function<void()>&& onJoin,
		std::function<std::string(ApiClient&, const std::string&)>&& onHost,
		std::function<void()>&& onTransition);

	tgui::Panel::Ptr CreateJoinPanel(
		tgui::Gui& gui,
		ApiClient& apiClient,
		sf::Sound& btnSound,
		std::function<void()>&& onBack,
		std::function<bool(ApiClient&, std::string)>&& onJoinLobby);

	void StyleButton(const tgui::Button::Ptr& button);
	void StyleSlider(const tgui::Slider::Ptr& slider);
};

