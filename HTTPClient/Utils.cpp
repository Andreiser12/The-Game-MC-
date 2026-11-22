module;
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <filesystem>

module Utils;

import HostPanel;
import SettingsState;

namespace Utils
{
	tgui::String TruncateText(const tgui::String& text, uint8_t maxChars)
	{
		if (text.length() < maxChars) return text;

		return text.substr(0, maxChars - 1) + "...";
	}

	tgui::Panel::Ptr Utils::CreateSettingsPanel(tgui::Gui& gui,
		SettingsState& state,
		sf::Sound& btnSound,
		std::function<void()> onClose,
		std::function<void()> onApply)
	{
		auto& resolutionIndex = state.resolutionIndex;
		auto& fullscreen = state.fullscreen;
		auto& framerateIndex = state.framerateIndex;

		auto& masterVol = state.masterVolume;
		auto& buttonsVol = state.buttonsVolume;
		auto& bgVol = state.backgroundVolume;
		uint8_t gameplayIndex{ 1u };

		auto panel = tgui::Panel::create({ "60%", "60%" });
		panel->setPosition("50%", "50%");
		panel->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 180));
		panel->getRenderer()->setBorders(2);
		panel->getRenderer()->setBorderColor(tgui::Color::Black);
		panel->setOrigin(0.5f, 0.5f);

		auto background = tgui::Picture::create("assets/images/utilsBackground.png");
		background->setSize({ "100%", "100%" });

		auto sidebar = tgui::Panel::create({ "20%", "100%" });
		sidebar->setPosition("0%", "0%");
		sidebar->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50, 180));
		sidebar->getRenderer()->setBorders(2);
		sidebar->getRenderer()->setBorderColor(tgui::Color::Black);
		sidebar->add(background);
		panel->add(sidebar);

		auto content = tgui::Panel::create({ "80%", "100%" });
		content->setPosition("20%", "0%");
		content->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
		panel->add(content);

		auto videoPanel = tgui::Panel::create({ "100%", "100%" });
		auto audioPanel = tgui::Panel::create({ "100%", "100%" });
		auto gameplayPanel = tgui::Panel::create({ "100%", "100%" });
		audioPanel->setVisible(false);
		gameplayPanel->setVisible(false);

		content->add(videoPanel);
		content->add(audioPanel);
		content->add(gameplayPanel);

		videoPanel->setPosition("0%", "0%");
		videoPanel->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
		auto vidioBackground = tgui::Picture::create("assets/images/utilsBackground.png");
		vidioBackground->setSize({ "100%", "100%" });
		videoPanel->add(vidioBackground);

		audioPanel->setPosition("0%", "0%");
		audioPanel->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
		auto audioBackground = tgui::Picture::create("assets/images/utilsBackground.png");
		audioBackground->setSize({ "100%", "100%" });
		audioPanel->add(audioBackground);

		gameplayPanel->setPosition("0%", "0%");
		gameplayPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
		gameplayPanel->add(background);

		auto videoButton = tgui::Button::create("Video");
		videoButton->setSize({ "90%", "10%" });
		videoButton->setPosition("5%", "20%");
		videoButton->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(videoButton);
		sidebar->add(videoButton);

		auto audioButton = tgui::Button::create("Audio");
		audioButton->setSize({ "90%", "10%" });
		audioButton->setPosition("5%", "35%");
		audioButton->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(audioButton);
		sidebar->add(audioButton);

		auto gameplayButton = tgui::Button::create("Gameplay");
		gameplayButton->setSize({ "90%", "10%" });
		gameplayButton->setPosition("5%", "50%");
		gameplayButton->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(gameplayButton);
		sidebar->add(gameplayButton);

		auto resolutionLabelBg = tgui::Panel::create({ "19%", "12.5%" });
		resolutionLabelBg->setPosition("5%", "10%");
		resolutionLabelBg->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		videoPanel->add(resolutionLabelBg);

		auto resolutionBgPicture = tgui::Picture::create("assets/images/settingsImage.png");
		resolutionBgPicture->setSize("100%", "100%");
		resolutionBgPicture->setPosition("0%", "0%");
		resolutionBgPicture->setIgnoreMouseEvents(true);
		resolutionLabelBg->add(resolutionBgPicture);

		auto resolutionText = tgui::Label::create("Resolution");
		resolutionText->setSize("90%", "45%");
		resolutionText->setPosition("50%", "50%");
		resolutionText->setOrigin(0.5f, 0.5f);
		resolutionText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		resolutionText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		resolutionText->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		resolutionText->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		resolutionText->getRenderer()->setTextOutlineThickness(1.5f);
		resolutionText->getRenderer()->setTextSize(24);
		resolutionLabelBg->add(resolutionText);

		auto resolutionContainer = tgui::Panel::create();
		resolutionContainer->setSize({ "40%", "12.5%" });
		resolutionContainer->setPosition("75%", "10%");
		resolutionContainer->setOrigin(0.5f, 0.f);
		resolutionContainer->getRenderer()->setBackgroundColor(tgui::Color::Transparent);

		auto resContainerBg = tgui::Picture::create("assets/images/settingsImage.png");
		resContainerBg->setSize("68%", "100%");
		resContainerBg->setPosition("16%", "0%");
		resContainerBg->setIgnoreMouseEvents(true);
		resolutionContainer->add(resContainerBg);

		auto resolutionValue = tgui::Label::create(
			std::to_string(SettingsState::Resolutions[resolutionIndex].x) + "x" +
			std::to_string(SettingsState::Resolutions[resolutionIndex].y)
		);
		resolutionValue->setSize("70%", "100%");
		resolutionValue->setPosition("15%", "0%");
		resolutionValue->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		resolutionValue->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		resolutionValue->getRenderer()->setBorders(2);
		resolutionValue->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		resolutionValue->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		resolutionValue->getRenderer()->setTextOutlineThickness(1.5f);
		resolutionValue->getRenderer()->setTextSize(24);

		auto leftResolutionArrow = tgui::Button::create("<");
		leftResolutionArrow->setSize({ "15%", "100%" });
		leftResolutionArrow->setPosition("0%", "0%");
		leftResolutionArrow->getRenderer()->setTexture("assets/images/buttons.png");
		Utils::StyleButton(leftResolutionArrow);

		auto rightResolutionArrow = tgui::Button::create(">");
		rightResolutionArrow->setSize({ "15%", "100%" });
		rightResolutionArrow->setPosition("85%", "0%");
		rightResolutionArrow->getRenderer()->setTexture("assets/images/buttons.png");
		Utils::StyleButton(rightResolutionArrow);
		rightResolutionArrow->getRenderer()->setTextSize(24);

		leftResolutionArrow->onClick([&resolutionIndex, resolutionValue, &btnSound]
			{
				btnSound.play();
				if (resolutionIndex == 0) resolutionIndex = static_cast<uint8_t>(SettingsState::Resolutions.size() - 1);
				else --resolutionIndex;
				resolutionValue->setText(std::to_string(SettingsState::Resolutions[resolutionIndex].x) + "x"
					+ std::to_string(SettingsState::Resolutions[resolutionIndex].y));
			});
		rightResolutionArrow->onClick([&resolutionIndex, resolutionValue, &btnSound]
			{
				btnSound.play();
				if (resolutionIndex == SettingsState::Resolutions.size() - 1)
					resolutionIndex = 0;
				else ++resolutionIndex;
				resolutionValue->setText(std::to_string(SettingsState::Resolutions[resolutionIndex].x) + "x"
					+ std::to_string(SettingsState::Resolutions[resolutionIndex].y));
			});

		resolutionContainer->add(leftResolutionArrow);
		resolutionContainer->add(resolutionValue);
		resolutionContainer->add(rightResolutionArrow);
		videoPanel->add(resolutionContainer);

		auto fullscreenLabelBg = tgui::Panel::create({ "19%", "12.5%" });
		fullscreenLabelBg->setPosition("5%", "35%");
		fullscreenLabelBg->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		videoPanel->add(fullscreenLabelBg);

		auto fullscreenBgPicture = tgui::Picture::create("assets/images/settingsImage.png");
		fullscreenBgPicture->setSize("100%", "100%");
		fullscreenBgPicture->setPosition("0%", "0%");
		fullscreenBgPicture->setIgnoreMouseEvents(true);
		fullscreenLabelBg->add(fullscreenBgPicture);

		auto fullscreenText = tgui::Label::create("Fullscreen");
		fullscreenText->setSize("90%", "45%");
		fullscreenText->setPosition("50%", "50%");
		fullscreenText->setOrigin(0.5f, 0.5f);
		fullscreenText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		fullscreenText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		fullscreenText->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		fullscreenText->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		fullscreenText->getRenderer()->setTextOutlineThickness(1.5f);
		fullscreenText->getRenderer()->setTextSize(24);
		fullscreenLabelBg->add(fullscreenText);

		auto fullscreenCheckbox = tgui::CheckBox::create();
		fullscreenCheckbox->setSize("5%", "6%");
		fullscreenCheckbox->setPosition("73%", "37.5%");
		fullscreenCheckbox->setChecked(true);
		fullscreenCheckbox->onCheck([&fullscreen, &btnSound] {
			btnSound.play();
			fullscreen = true;
			});
		fullscreenCheckbox->onUncheck([&fullscreen, &btnSound] {
			btnSound.play();
			fullscreen = false;
			});
		videoPanel->add(fullscreenCheckbox);

		auto framerateLabelBg = tgui::Panel::create({ "19%", "12.5%" });
		framerateLabelBg->setPosition("5%", "60%");
		framerateLabelBg->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		videoPanel->add(framerateLabelBg);

		auto framerateBgPicture = tgui::Picture::create("assets/images/settingsImage.png");
		framerateBgPicture->setSize("100%", "100%");
		framerateBgPicture->setPosition("0%", "0%");
		framerateBgPicture->setIgnoreMouseEvents(true);
		framerateLabelBg->add(framerateBgPicture);

		auto framerateText = tgui::Label::create("Framerate");
		framerateText->setSize("90%", "45%");
		framerateText->setPosition("50%", "50%");
		framerateText->setOrigin(0.5f, 0.5f);
		framerateText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		framerateText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		framerateText->getRenderer()->setTextSize(24);
		framerateText->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		framerateText->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		framerateText->getRenderer()->setTextOutlineThickness(1.5f);
		framerateLabelBg->add(framerateText);

		auto framerateContainer = tgui::Panel::create();
		framerateContainer->setSize({ "40%", "12.5%" });
		framerateContainer->setPosition("75%", "60%");
		framerateContainer->setOrigin(0.5f, 0.f);
		framerateContainer->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));

		auto frameContainerBg = tgui::Picture::create("assets/images/settingsImage.png");
		frameContainerBg->setSize("68%", "100%");
		frameContainerBg->setPosition("16%", "0%");
		frameContainerBg->setIgnoreMouseEvents(true);
		framerateContainer->add(frameContainerBg);

		auto framerateValue = tgui::Label::create(std::to_string((SettingsState::Framerates[framerateIndex])));
		framerateValue->setSize("70%", "100%");
		framerateValue->setPosition("15%", "0%");
		framerateValue->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		framerateValue->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		framerateValue->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		framerateValue->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		framerateValue->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		framerateValue->getRenderer()->setTextOutlineThickness(1.5f);
		framerateValue->getRenderer()->setTextSize(24);

		auto leftFramerateArrow = tgui::Button::create("<");
		leftFramerateArrow->setSize({ "15%", "100%" });
		leftFramerateArrow->setPosition("0%", "0%");
		leftFramerateArrow->getRenderer()->setTexture("assets/images/buttons.png");
		Utils::StyleButton(leftFramerateArrow);
		leftFramerateArrow->onClick([&framerateIndex, framerateValue, &btnSound] {
			btnSound.play();
			if (framerateIndex == 0) framerateIndex = static_cast<uint8_t>(SettingsState::Framerates.size() - 1);
			else --framerateIndex;
			framerateValue->setText(std::to_string(SettingsState::Framerates[framerateIndex]));
			});

		auto rightFramerateArrow = tgui::Button::create(">");
		rightFramerateArrow->setSize({ "15%", "100%" });
		rightFramerateArrow->setPosition({ "85%", "0%" });
		rightFramerateArrow->getRenderer()->setTexture("assets/images/buttons.png");
		Utils::StyleButton(rightFramerateArrow);
		rightFramerateArrow->onClick([&framerateIndex, framerateValue, &btnSound] {
			btnSound.play();
			if (framerateIndex == SettingsState::Framerates.size() - 1) framerateIndex = 0;
			else ++framerateIndex;
			framerateValue->setText(std::to_string(SettingsState::Framerates[framerateIndex]));
			});

		framerateContainer->add(framerateValue);
		framerateContainer->add(leftFramerateArrow);
		framerateContainer->add(rightFramerateArrow);
		videoPanel->add(framerateContainer);

		auto masterLabelBg = tgui::Panel::create({ "25%", "8%" });
		masterLabelBg->setPosition("5%", "11.5%");
		masterLabelBg->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		audioPanel->add(masterLabelBg);

		auto masterBgPicture = tgui::Picture::create("assets/images/settingsImage.png");
		masterBgPicture->setSize("100%", "100%");
		masterBgPicture->setPosition("0%", "0%");
		masterBgPicture->setIgnoreMouseEvents(true);
		masterLabelBg->add(masterBgPicture);

		auto masterLabel = tgui::Label::create("Master Volume");
		masterLabel->setSize({ "100%", "100%" });
		masterLabel->setPosition("0%", "0%");
		masterLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		masterLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		masterLabel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		masterLabel->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		masterLabel->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		masterLabel->getRenderer()->setTextOutlineThickness(1.5f);
		masterLabel->getRenderer()->setTextSize(22);
		masterLabelBg->add(masterLabel);

		auto masterValueLabel = tgui::Label::create(std::to_string(static_cast<int>(masterVol)) + "%");
		masterValueLabel->setSize({ "9%", "8%" });
		masterValueLabel->setPosition("90%", "11.5%");
		masterValueLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		masterValueLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		masterValueLabel->getRenderer()->setBorders(2);
		masterValueLabel->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50, 180));
		masterValueLabel->getRenderer()->setBorderColor(tgui::Color::Black);
		masterValueLabel->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		masterValueLabel->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		masterValueLabel->getRenderer()->setTextOutlineThickness(1.f);
		masterValueLabel->getRenderer()->setTextSize(20);
		audioPanel->add(masterValueLabel);

		auto masterSlider = tgui::Slider::create();
		masterSlider->setPosition({ "32.5%", "14.5%" });
		masterSlider->setSize({ "55%", "2%" });
		masterSlider->setMinimum(0);
		masterSlider->setMaximum(100);
		masterSlider->setValue(masterVol);
		StyleSlider(masterSlider);
		masterSlider->onValueChange([&masterVol, masterValueLabel](float value) {
			masterVol = value;
			masterValueLabel->setText(std::to_string(static_cast<int>(value)) + "%");
			});
		audioPanel->add(masterSlider);

		auto buttonsLabelBg = tgui::Panel::create({ "25%", "8%" });
		buttonsLabelBg->setPosition("5%", "36.5%");
		buttonsLabelBg->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		buttonsLabelBg->getRenderer()->setTextureBackground("assets/images/settingsImage.png");
		audioPanel->add(buttonsLabelBg);

		auto buttonsLabel = tgui::Label::create("Buttons Volume");
		buttonsLabel->setSize({ "100%", "100%" });
		buttonsLabel->setPosition("0%", "0%");
		buttonsLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		buttonsLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		buttonsLabel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		buttonsLabel->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		buttonsLabel->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		buttonsLabel->getRenderer()->setTextOutlineThickness(1.5f);
		buttonsLabel->getRenderer()->setTextSize(22);
		buttonsLabelBg->add(buttonsLabel);

		auto buttonsValueLabel = tgui::Label::create(std::to_string(static_cast<int>(buttonsVol)) + "%");
		buttonsValueLabel->setSize({ "9%", "8%" });
		buttonsValueLabel->setPosition("90%", "36.5%");
		buttonsValueLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		buttonsValueLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		buttonsValueLabel->getRenderer()->setBorders(2);
		buttonsValueLabel->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50, 180));
		buttonsValueLabel->getRenderer()->setBorderColor(tgui::Color::Black);
		buttonsValueLabel->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		buttonsValueLabel->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		buttonsValueLabel->getRenderer()->setTextOutlineThickness(1.f);
		buttonsValueLabel->getRenderer()->setTextSize(20);
		audioPanel->add(buttonsValueLabel);

		auto buttonsSlider = tgui::Slider::create();
		buttonsSlider->setPosition({ "32.5%", "39.5%" });
		buttonsSlider->setSize({ "55%", "2%" });
		buttonsSlider->setMinimum(0);
		buttonsSlider->setMaximum(100);
		buttonsSlider->setValue(buttonsVol);
		StyleSlider(buttonsSlider);
		buttonsSlider->onValueChange([&buttonsVol, buttonsValueLabel](float value) {
			buttonsVol = value;
			buttonsValueLabel->setText(std::to_string(static_cast<int>(value)) + "%");
			});
		audioPanel->add(buttonsSlider);

		auto bgLabelBg = tgui::Panel::create({ "25%", "8%" });
		bgLabelBg->setPosition("5%", "61.5%");
		bgLabelBg->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		bgLabelBg->getRenderer()->setTextureBackground("assets/images/settingsImage.png");
		audioPanel->add(bgLabelBg);

		auto bgLabel = tgui::Label::create("Background Vol.");
		bgLabel->setSize({ "100%", "100%" });
		bgLabel->setPosition("0%", "0%");
		bgLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		bgLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		bgLabel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		bgLabel->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		bgLabel->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		bgLabel->getRenderer()->setTextOutlineThickness(1.5f);
		bgLabel->getRenderer()->setTextSize(22);
		bgLabelBg->add(bgLabel);

		auto bgValueLabel = tgui::Label::create(std::to_string(static_cast<int>(bgVol)) + "%");
		bgValueLabel->setSize({ "9%", "8%" });
		bgValueLabel->setPosition("90%", "61.5%");
		bgValueLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		bgValueLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		bgValueLabel->getRenderer()->setBorders(2);
		bgValueLabel->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50, 180));
		bgValueLabel->getRenderer()->setBorderColor(tgui::Color::Black);
		bgValueLabel->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		bgValueLabel->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		bgValueLabel->getRenderer()->setTextOutlineThickness(1.f);
		bgValueLabel->getRenderer()->setTextSize(20);
		audioPanel->add(bgValueLabel);

		auto bgSlider = tgui::Slider::create();
		bgSlider->setPosition({ "32.5%", "64.5%" });
		bgSlider->setSize({ "55%", "2%" });
		bgSlider->setMinimum(0);
		bgSlider->setMaximum(100);
		bgSlider->setValue(bgVol);
		StyleSlider(bgSlider);
		bgSlider->onValueChange([&bgVol, bgValueLabel](float value) {
			bgVol = value;
			bgValueLabel->setText(std::to_string(static_cast<int>(value)) + "%");
			});
		audioPanel->add(bgSlider);

		auto gameplayPicture = tgui::Picture::create("assets/images/gameplayImage1.png");
		gameplayPicture->setSize("70%", "70%");
		gameplayPicture->setPosition("50%", "45%");
		gameplayPicture->setOrigin(0.5f, 0.5f);
		gameplayPanel->add(gameplayPicture);

		auto leftArrow = tgui::Button::create("<");
		leftArrow->setSize({ "10%", "10%" });
		leftArrow->setPosition("2%", "45%");
		leftArrow->setOrigin(0.f, 0.5f);
		leftArrow->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(leftArrow);
		leftArrow->setVisible(false);
		gameplayPanel->add(leftArrow);

		auto rightArrow = tgui::Button::create(">");
		rightArrow->setSize({ "10%", "10%" });
		rightArrow->setPosition("98%", "45%");
		rightArrow->setOrigin(1.f, 0.5f);
		rightArrow->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(rightArrow);
		gameplayPanel->add(rightArrow);

		auto updateGameplayState = [&gameplayIndex, gameplayPicture, leftArrow, rightArrow]() {
			try {
				std::string path = "assets/images/gameplayImage" + std::to_string(gameplayIndex) + ".png";
				gameplayPicture->getRenderer()->setTexture(tgui::Texture(path));
			}
			catch (...) {
			}

			leftArrow->setVisible(gameplayIndex > 1);
			rightArrow->setVisible(gameplayIndex < 6);
			};

		leftArrow->onClick([&gameplayIndex, &updateGameplayState, &btnSound]() {
			btnSound.play();
			if (gameplayIndex > 1) {
				--gameplayIndex;
				updateGameplayState();
			}
			});

		rightArrow->onClick([&gameplayIndex, &updateGameplayState, &btnSound]() {
			btnSound.play();
			if (gameplayIndex < 6) {
				++gameplayIndex;
				updateGameplayState();
			}
			});

		auto applyButton = tgui::Button::create("Apply");
		applyButton->setSize({ "15%", "10%" });
		applyButton->setPosition("75%", "85%");
		applyButton->getRenderer()->setBorders(2);
		applyButton->getRenderer()->setRoundedBorderRadius(10);
		applyButton->getRenderer()->setTexture("assets/images/buttons.png");
		Utils::StyleButton(applyButton);
		applyButton->onClick([onApply, &btnSound] {
			btnSound.play();
			if (onApply) onApply();
			});
		content->add(applyButton);

		auto closeButton = tgui::Button::create("Close");
		closeButton->setSize({ "15%", "10%" });
		closeButton->setPosition("10%", "85%");
		closeButton->getRenderer()->setTextSize(24);
		closeButton->getRenderer()->setBorders(2);
		closeButton->getRenderer()->setRoundedBorderRadius(10);
		closeButton->getRenderer()->setTexture("assets/images/buttons.png");
		Utils::StyleButton(closeButton);
		closeButton->onClick([panel, onClose] {
			panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
			if (onClose) onClose();
			});
		content->add(closeButton);

		videoButton->onClick([=, &btnSound] {
			btnSound.play();
			videoPanel->setVisible(true);
			audioPanel->setVisible(false);
			gameplayPanel->setVisible(false);
			applyButton->setVisible(true);
			});

		audioButton->onClick([=, &btnSound] {
			btnSound.play();
			videoPanel->setVisible(false);
			audioPanel->setVisible(true);
			gameplayPanel->setVisible(false);
			applyButton->setVisible(true);
			});

		gameplayButton->onClick([=, &btnSound] {
			btnSound.play();
			videoPanel->setVisible(false);
			audioPanel->setVisible(false);
			gameplayPanel->setVisible(true);
			applyButton->setVisible(false);
			});
		return panel;
	}

	tgui::Panel::Ptr Utils::CreateProfilePanel(
		const UserProfile& profile,
		sf::Sound& btnSound,
		std::function<void()> onClose,
		std::function<void()> onDeleteAccount,
		std::function<void()> onChangeAvatar)
	{
		auto panel = tgui::Panel::create({ "60%", "60%" });
		panel->setPosition({ "20%", "20%" });

		auto title = tgui::Label::create("PLAYER PROFILE");
		title->getRenderer()->setTextStyle(tgui::TextStyle::Bold);
		title->getRenderer()->setTextSize(32);
		title->setPosition({ "50%", "6%" });
		title->setOrigin(0.5f, 0.f);
		panel->add(title);

		auto line = tgui::SeparatorLine::create();
		line->setPosition({ "5%", "13%" });
		line->setSize({ "90%", "2" });
		panel->add(line);

		auto username = tgui::Label::create("Username: " + profile.username);
		username->setPosition({ "5%", "22%" });
		username->getRenderer()->setTextSize(24);
		panel->add(username);

		auto gamesPlayed = tgui::Label::create("Games played: " + std::to_string(profile.gamesPlayed));
		gamesPlayed->setPosition({ "5%", "34%" });
		gamesPlayed->getRenderer()->setTextSize(24);
		panel->add(gamesPlayed);

		auto gamesWon = tgui::Label::create("Games won: " + std::to_string(profile.gamesWon));
		gamesWon->setPosition({ "5%", "46%" });
		gamesWon->getRenderer()->setTextSize(24);
		panel->add(gamesWon);

		std::ostringstream outstream;
		outstream << std::fixed << std::setprecision(1) << profile.hoursPlayed;

		auto hoursPlayed = tgui::Label::create("Hours played: " + outstream.str());
		hoursPlayed->setPosition({ "5%", "58%" });
		hoursPlayed->getRenderer()->setTextSize(24);
		panel->add(hoursPlayed);

		outstream.str("");
		outstream.clear();

		double performanceScore;
		if (profile.gamesPlayed == 0) performanceScore = 1.0;
		else performanceScore = 1.0 + (static_cast<double>(profile.gamesWon) / profile.gamesPlayed) * 4.0;
		outstream << std::fixed << std::setprecision(1) << performanceScore;

		auto performance = tgui::Label::create("Performance score: " + outstream.str());
		performance->setPosition({ "5%", "70%" });
		performance->getRenderer()->setTextSize(24);
		panel->add(performance);
		outstream.str("");
		outstream.clear();

		auto deleteButton = tgui::Button::create("Delete Account");
		deleteButton->setSize({ "23%", "8%" });
		deleteButton->setPosition({ "72%", "85%" });
		deleteButton->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(deleteButton);
		deleteButton->getRenderer()->setTextSize(18);
		deleteButton->onPress(
			[&btnSound, onDeleteAccount]
			{
				btnSound.play();
				if (onDeleteAccount)
					onDeleteAccount();
			});
		panel->add(deleteButton);

		auto avatar = tgui::Picture::create();
		tgui::Texture avatarTexture;
		const std::string defaultAvatar = "assets/images/playerDefaultAvatar.jpg";
		sf::Image img;
		if (!profile.avatarPath.empty())
		{
			std::filesystem::path localAvatar =
				std::filesystem::path("assets/avatars") /
				std::filesystem::path(profile.avatarPath).filename();

			if (std::filesystem::exists(localAvatar) && img.loadFromFile(localAvatar.string()))
				avatarTexture.loadFromPixelData({ img.getSize().x, img.getSize().y }, img.getPixelsPtr());
			else if(img.loadFromFile(defaultAvatar))
				avatarTexture.loadFromPixelData({ img.getSize().x, img.getSize().y }, img.getPixelsPtr());
		}
		else if (img.loadFromFile(defaultAvatar))
			avatarTexture.loadFromPixelData({ img.getSize().x, img.getSize().y }, img.getPixelsPtr());
		avatar->getRenderer()->setTexture(avatarTexture);
		avatar->setSize({ "22%", "30%" });
		avatar->setPosition({ "72%", "38%" });
		avatar->setOrigin(0.5f, 0.5f);
		panel->add(avatar);

		auto changeAvatarButton = tgui::Button::create("Change Avatar");
		changeAvatarButton->setSize({ "25%", "8%" });
		changeAvatarButton->setPosition({ "72%", "58%" });
		changeAvatarButton->setOrigin(0.5f, 0.5f);
		changeAvatarButton->getRenderer()->setTexture("assets/images/buttons.png");
		changeAvatarButton->getRenderer()->setBorders(1);
		StyleButton(changeAvatarButton);
		changeAvatarButton->getRenderer()->setTextSize(18);
		changeAvatarButton->onPress(
			[&btnSound, onChangeAvatar]()
			{
				btnSound.play();
				if (onChangeAvatar)
					onChangeAvatar();
			});
		panel->add(changeAvatarButton);

		auto closeButton = tgui::Button::create("X");
		closeButton->setSize({ "8%", "10%" });
		closeButton->getRenderer()->setTexture("assets/images/buttons.png");
		closeButton->getRenderer()->setBorders(1);
		StyleButton(closeButton);
		closeButton->getRenderer()->setTextSize(22);
		closeButton->setOrigin(1.f, 0.f);
		closeButton->setPosition({ "98%", "2%" });
		closeButton->onPress(
			[panel, onClose, &btnSound]
			{
				btnSound.play();
				panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
				if (onClose) onClose();
			});
		panel->add(closeButton);

		panel->getRenderer()->setBackgroundColor(tgui::Color(255, 255, 255, 210));
		panel->getRenderer()->setBorders(2);
		panel->getRenderer()->setBorderColor(tgui::Color(60, 60, 60));
		panel->getRenderer()->setRoundedBorderRadius(18);

		return panel;
	}

	tgui::Panel::Ptr CreateExitPanel(tgui::Gui& gui,
		sf::Sound& btnSound,
		std::function<void()> onYes,
		std::function<void()> onNo)
	{
		auto panel = tgui::Panel::create({ "33.33%", "33.33%" });
		panel->setPosition({ "50%", "50%" });
		panel->setOrigin(0.5f, 0.5f);
		panel->getRenderer()->setBorders(2);
		panel->getRenderer()->setBorderColor(tgui::Color::Black);

		auto background = tgui::Picture::create("assets/images/mainMenuBackground.png");
		background->setSize({ "100%", "100%" });
		panel->add(background);

		auto textLabel = tgui::Label::create("Are you sure you want to exit?");
		textLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		textLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		textLabel->setPosition({ "50%", "45%" });
		textLabel->setOrigin(0.5f, 0.5f);
		textLabel->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		textLabel->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		textLabel->getRenderer()->setTextOutlineThickness(1.5f);
		textLabel->getRenderer()->setTextSize(24);

		auto noButton = tgui::Button::create("No");
		noButton->setSize({ "15%", "20%" });
		noButton->setPosition({ "10%", "70%" });
		noButton->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(noButton);
		noButton->getRenderer()->setTextSize(20);
		noButton->onPress([panel, onNo, &btnSound]() {
			btnSound.play();
			panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(100));
			if (onNo) onNo();
			});

		auto yesButton = tgui::Button::create("Yes");
		yesButton->setSize({ "15%", "20%" });
		yesButton->setPosition({ "75%", "70%" });
		yesButton->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(yesButton);
		yesButton->getRenderer()->setTextSize(20);
		yesButton->onPress([panel, onYes, &btnSound]() {
			btnSound.play();
			if (onYes) onYes();
			});

		panel->add(textLabel);
		panel->add(noButton);
		panel->add(yesButton);
		return panel;
	}

	tgui::Panel::Ptr Utils::CreateDeleteAccountPanel(tgui::Gui& gui, sf::Sound& btnSound, std::function<void()> onYes, std::function<void()> onNo)
	{
		auto panel = tgui::Panel::create({ "33.33%", "33.33%" });
		panel->setPosition({ "50%", "50%" });
		panel->setOrigin(0.5f, 0.5f);
		panel->getRenderer()->setBorders(2);
		panel->getRenderer()->setBorderColor(tgui::Color::Black);

		auto background = tgui::Picture::create("assets/images/mainMenuBackground.png");
		background->setSize({ "100%", "100%" });
		panel->add(background);

		auto textLabel = tgui::Label::create("Are you sure you want to delete your account?\nYou cannot undo your decision.");
		textLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		textLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		textLabel->setPosition({ "50%", "45%" });
		textLabel->setOrigin(0.5f, 0.5f);
		textLabel->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		textLabel->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		textLabel->getRenderer()->setTextOutlineThickness(1.5f);
		textLabel->getRenderer()->setTextSize(24);

		auto noButton = tgui::Button::create("No");
		noButton->setSize({ "15%", "20%" });
		noButton->setPosition({ "10%", "70%" });
		noButton->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(noButton);
		noButton->getRenderer()->setTextSize(20);
		noButton->onPress([panel, onNo, &btnSound]() {
			btnSound.play();
			panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(100));
			if (onNo) onNo();
			});

		auto yesButton = tgui::Button::create("Yes");
		yesButton->setSize({ "15%", "20%" });
		yesButton->setPosition({ "75%", "70%" });
		yesButton->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(yesButton);
		yesButton->getRenderer()->setTextSize(20);
		yesButton->onPress([panel, onYes, &btnSound]() {
			btnSound.play();
			if (onYes) onYes();
			});

		panel->add(textLabel);
		panel->add(noButton);
		panel->add(yesButton);
		return panel;
	}

	tgui::Panel::Ptr CreateErrorPopupPanel(tgui::Gui& gui,
		const std::string& message, sf::Sound& btnSound, std::function<void()> onClose)
	{
		auto errorPopupPanel = tgui::Panel::create({ "25%", "25%" });
		errorPopupPanel->setPosition({ "50%", "50%" });
		errorPopupPanel->setOrigin(0.5f, 0.5f);
		errorPopupPanel->getRenderer()->setBorderColor(tgui::Color::Black);
		errorPopupPanel->getRenderer()->setBorders(1);

		auto background = tgui::Picture::create("assets/images/mainMenuBackground.png");
		background->setSize({ "100%", "100%" });
		errorPopupPanel->add(background);

		auto okButton = tgui::Button::create("OK");
		okButton->setSize({ "25%", "25%" });
		okButton->setPosition({ "50%", "75%" });
		okButton->setOrigin(0.5f, 0.5f);
		okButton->getRenderer()->setTexture("assets/images/buttons.png");
		StyleButton(okButton);
		okButton->onPress([errorPopupPanel, onClose, &btnSound]()
			{
				btnSound.play();
				errorPopupPanel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
				if (onClose) onClose();
			});
		errorPopupPanel->add(okButton);

		auto errorLabel = tgui::Label::create(message);
		errorLabel->setSize({ "60%", "40%" });
		errorLabel->setPosition({ "50%", "45%" });
		errorLabel->getRenderer()->setTextSize(24);
		errorLabel->setOrigin(0.5f, 0.5f);
		errorLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		errorLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		errorPopupPanel->add(errorLabel);

		return errorPopupPanel;
	}

	tgui::Panel::Ptr CreatePlayPanel(tgui::Gui& gui, ApiClient& apiClient, sf::Sound& btnSound,
		std::function<void()>&& onClose, std::function<void()>&& onJoin, 
		std::function<std::string(ApiClient&, const std::string&)>&& onHost, std::function<void()>&& onTransition)
	{

		auto background = tgui::Picture::create("assets/images/mainMenuBackground.png");
		background->setSize("100%", "100%");
		background->setPosition("0%", "0%");

		auto panel = tgui::Panel::create({ "30%", "25%" });
		panel->setPosition("50%", "50%");
		panel->getRenderer()->setBorders(2);
		panel->getRenderer()->setBorderColor(tgui::Color::Black);
		panel->setOrigin(0.5f, 0.5f);

		auto titleLabel = tgui::Label::create("Select an option");
		titleLabel->setPosition("50%", "44%");
		titleLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		titleLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		titleLabel->setOrigin(0.5f, 0.5f);
		titleLabel->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		titleLabel->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		titleLabel->getRenderer()->setTextOutlineThickness(1.5f);
		titleLabel->getRenderer()->setTextSize(26);

		auto joinButton = tgui::Button::create("Join Lobby");
		joinButton->getRenderer()->setTexture("assets/images/buttons.png");
		joinButton->setSize("35%", "20%");
		joinButton->setPosition("5%", "70%");
		Utils::StyleButton(joinButton);
		joinButton->getRenderer()->setTextSize(22);
		joinButton->onPress([&gui, &apiClient, &btnSound, panel, onTransition, onJoin]() {
			btnSound.play();
			panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
			auto joinPanel = CreateJoinPanel(
				gui,
				apiClient,
				btnSound,
				[panel]() { panel->setVisible(true); },
				[&gui, &apiClient, &btnSound, panel, onTransition](ApiClient& api, std::string lobbyIdEntered)
				{
					panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
					std::string lobbyId{ api.GetLobbyId() };
					auto host = HostPanel::Create(
						gui,
						apiClient,
						btnSound,
						lobbyId,
						[lobbyId](ApiClient& api) { return api.LeaveLobby(lobbyId, api.GetPlayerId()); },
						[lobbyId, onTransition](ApiClient& api) {
							bool success{ api.StartMatch(lobbyId, api.GetPlayerId()) };
							if (success && onTransition) onTransition();
							return success;
						}
					);
					gui.add(host, "HostPanel");
					return true;
				}
			);
			gui.add(joinPanel);
			if (onJoin) onJoin();
			});

		auto hostButton = tgui::Button::create("Host Lobby");
		hostButton->setSize("35%", "20%");
		hostButton->setPosition("60%", "70%");
		hostButton->getRenderer()->setTexture("assets/images/buttons.png");
		Utils::StyleButton(hostButton);
		hostButton->getRenderer()->setTextSize(22);
		hostButton->onPress([&gui, &apiClient, &btnSound, onHost, onTransition, panel]() {
			btnSound.play();
			if (onHost)
			{
				std::string lobbyId{ onHost(apiClient, apiClient.GetUsername()) };
				apiClient.SetLobbyId(lobbyId);
				panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
				auto hostPanel = HostPanel::Create(
					gui,
					apiClient,
					btnSound,
					lobbyId,
					[lobbyId](ApiClient& api) { return api.LeaveLobby(lobbyId, api.GetPlayerId()); },
					[lobbyId, onTransition](ApiClient& api) {
						bool success = api.StartMatch(lobbyId, api.GetPlayerId());
						if (success && onTransition) onTransition();
						return success;
					}
				);
				gui.add(hostPanel, "HostPanel");
			}
			});

		auto closeButton = tgui::Button::create("X");
		closeButton->setSize("10%", "10%");
		closeButton->setPosition("90%", "0%");
		closeButton->getRenderer()->setTexture("assets/images/buttons.png");
		closeButton->getRenderer()->setBorders(1);
		Utils::StyleButton(closeButton);
		closeButton->getRenderer()->setTextSize(16);
		closeButton->onPress([panel, onClose, &btnSound]()
			{
				btnSound.play();
				panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
				if (onClose) onClose();
			});

		panel->add(background);
		panel->add(closeButton);
		panel->add(joinButton);
		panel->add(hostButton);
		panel->add(titleLabel);
		return panel;

	}

	tgui::Panel::Ptr CreateJoinPanel(tgui::Gui& gui, ApiClient& apiClient, sf::Sound& btnSound, 
		std::function<void()>&& onBack, std::function<bool(ApiClient&, std::string)>&& onJoinLobby)
	{
		auto background = tgui::Picture::create("assets/images/mainMenuBackground.png");
		background->setSize("100%", "100%");

		auto panel = tgui::Panel::create({ "30%", "25%" });
		panel->setPosition("50%", "50%");
		panel->setOrigin(0.5f, 0.5f);
		panel->getRenderer()->setBorders(2);
		panel->getRenderer()->setBorderColor(tgui::Color::Black);

		auto lobbyEditBox = tgui::EditBox::create();
		lobbyEditBox->setSize("45%", "20%");
		lobbyEditBox->setPosition("50%", "42%");
		lobbyEditBox->getRenderer()->setTexture("assets/images/textBoxbackground.png");
		lobbyEditBox->getRenderer()->setFont("assets/fonts/Sanchez-Regular.ttf");
		lobbyEditBox->setDefaultText("Enter lobby ID");
		lobbyEditBox->setOrigin(0.5f, 0.5f);
		lobbyEditBox->getRenderer()->setTextSize(26);
		lobbyEditBox->setMaximumCharacters(6);

		lobbyEditBox->onFocus([lobbyEditBox]() {
			lobbyEditBox->setDefaultText("");
			});
		lobbyEditBox->onUnfocus([lobbyEditBox]()
			{
				lobbyEditBox->setDefaultText("Enter lobby ID");
			});

		auto joinLobbyButton = tgui::Button::create("Join");
		joinLobbyButton->getRenderer()->setTexture("assets/images/buttons.png");
		joinLobbyButton->setSize("35%", "20%");
		joinLobbyButton->setPosition("60%", "70%");
		Utils::StyleButton(joinLobbyButton);
		joinLobbyButton->getRenderer()->setTextSize(22);
		joinLobbyButton->onPress([&gui, onJoinLobby, &apiClient, &btnSound, lobbyEditBox, panel]()
			{
				btnSound.play();
				const std::string lobbyId{ lobbyEditBox->getText().toStdString() };

				if (lobbyId.empty())
				{
					gui.add(Utils::CreateErrorPopupPanel(gui, "Lobby ID can't be empty.", btnSound));
					return;
				}
				if (lobbyId.size() != 6)
				{
					gui.add(Utils::CreateErrorPopupPanel(gui, "Lobby ID must be 6 characters long.", btnSound));
					return;
				}

				JoinLobbyResult joinLobbyResult = apiClient.JoinLobbyValidated(lobbyId,
					apiClient.GetUsername());

				switch (joinLobbyResult)
				{
				case JoinLobbyResult::Success:
					if (onJoinLobby) onJoinLobby(apiClient, lobbyId);
					panel->setVisible(false);
					break;

				case JoinLobbyResult::LobbyNotFound:
					gui.add(Utils::CreateErrorPopupPanel(gui,
						"Lobby not found. Check the ID and try again.", btnSound));
					break;

				case JoinLobbyResult::LobbyFull:
					gui.add(Utils::CreateErrorPopupPanel(gui,
						"Lobby is full. Try another lobby", btnSound));
					break;

				case JoinLobbyResult::GameAlreadyStarted:
					gui.add(Utils::CreateErrorPopupPanel(gui,
						"Game has already started. You can't join now.", btnSound));
					break;

				case JoinLobbyResult::NetworkError:
					gui.add(Utils::CreateErrorPopupPanel(gui,
						"Network error. Please try again.", btnSound));
					break;
				}
			});

		auto backButton = tgui::Button::create("Back");
		backButton->setSize("35%", "20%");
		backButton->setPosition("10%", "70%");
		backButton->getRenderer()->setTexture("assets/images/buttons.png");
		Utils::StyleButton(backButton);
		backButton->getRenderer()->setTextSize(22);
		backButton->onPress([&apiClient, &btnSound, panel, onBack]()
			{
				btnSound.play();
				panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
				if (onBack) onBack();
			});

		panel->add(background);
		panel->add(lobbyEditBox);
		panel->add(backButton);
		panel->add(joinLobbyButton);
		return panel;
	}

	void StyleButton(const tgui::Button::Ptr& button)
	{
		button->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		button->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		button->getRenderer()->setTextOutlineThickness(1.5f);
		button->getRenderer()->setOpacity(0.7f);
		button->getRenderer()->setTextSize(24);
	}

	void StyleSlider(const tgui::Slider::Ptr& slider)
	{
		auto renderer = slider->getRenderer();
		renderer->setTrackColor(tgui::Color::Black);
		renderer->setTrackColorHover(tgui::Color::Black);
		renderer->setBorders(0);
		renderer->setThumbColor(tgui::Color(245, 240, 230));
		renderer->setThumbColorHover(tgui::Color::White);
	}


}