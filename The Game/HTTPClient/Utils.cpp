module;
#define NOMINMAX
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "SFML/Graphics.hpp"
#include <SFML/Audio.hpp>
#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

module Utils;

import UserProfile;

tgui::String Utils::TruncateText(const tgui::String& text, uint8_t maxChars)
{
	if (text.length() < maxChars) return text;

	return text.substr(0, maxChars - 1) + "...";
}

tgui::Panel::Ptr Utils::CreateSettingsPanel(tgui::Gui& gui,
	unsigned resolutionIndex,
	bool isFullscreen,
	unsigned framerateIndex,
	float masterVolume,
	float buttonsVolume,
	float backgroundVolume,
	sf::Sound& btnSound,
	std::function<void()> onClose,
	std::function<void(unsigned res, bool fullscreen, unsigned framerate,
		float masterVol, float btnVol, float bgVol)> onApply)
{
	auto resolutionsIndexPtr = std::make_shared<unsigned>(resolutionIndex);
	auto resolutions = std::make_shared<std::vector<sf::Vector2u>>
		(std::vector<sf::Vector2u>{
			{640, 480},
			{ 800, 600 },
			{ 1024, 768 },
			{ 1280, 720 },
			{ 1280, 800 },
			{ 1366, 768 },
			{ 1440, 900 },
			{ 1680, 1050 },
			{ 1920, 1080 },
			{ 1920, 1200 },
			{ 2560, 1440 }
	});
	auto fullscreenPtr = std::make_shared<bool>(isFullscreen);
	auto framerateIndexPtr = std::make_shared<unsigned>(framerateIndex);
	auto framerates = std::make_shared<std::vector<unsigned>>(
		std::vector<unsigned>
	{
		30u,
			60u,
			120u,
			144u,
			180u
	});

	auto masterVolPtr = std::make_shared<float>(masterVolume);
	auto buttonsVolPtr = std::make_shared<float>(buttonsVolume);
	auto bgVolPtr = std::make_shared<float>(backgroundVolume);

	auto panel = tgui::Panel::create({ "60%", "60%" });
	panel->setPosition("50%", "50%");
	panel->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 180));
	panel->getRenderer()->setBorders(2);
	panel->getRenderer()->setBorderColor(tgui::Color::Black);
	panel->setOrigin(0.5f, 0.5f);

	auto background = tgui::Picture::create("assets/images/mainMenuBackground.png");
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
	audioPanel->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));

	audioPanel->setPosition("0%", "0%");
	audioPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	audioPanel->add(background);

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

	videoButton->onClick([=, &btnSound] {
		btnSound.play();
		videoPanel->setVisible(true);
		audioPanel->setVisible(false);
		gameplayPanel->setVisible(false);
		});

	audioButton->onClick([=, &btnSound] {
		btnSound.play();
		videoPanel->setVisible(false);
		audioPanel->setVisible(true);
		gameplayPanel->setVisible(false);
		});

	gameplayButton->onClick([=, &btnSound] {
		btnSound.play();
		videoPanel->setVisible(false);
		audioPanel->setVisible(false);
		gameplayPanel->setVisible(true);
		});

	auto resolutionContainer = tgui::Panel::create();
	resolutionContainer->setSize({ "40%", "12.5%" });
	resolutionContainer->setPosition("75%", "10%");
	resolutionContainer->setOrigin(0.5f, 0.f);
	resolutionContainer->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));

	auto resolutionText = tgui::Label::create("Resolution");
	resolutionText->setSize("16%", "12.5%");
	resolutionText->setPosition("5%", "10%");
	resolutionText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	resolutionText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	resolutionText->getRenderer()->setBorders(2);
	resolutionText->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
	resolutionText->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
	resolutionText->getRenderer()->setTextOutlineThickness(1.5f);
	resolutionText->getRenderer()->setTextSize(24);
	videoPanel->add(resolutionText);

	auto resolutionValue = tgui::Label::create(
		std::to_string((*resolutions)[*resolutionsIndexPtr].x) + "x" +
		std::to_string((*resolutions)[*resolutionsIndexPtr].y)
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

	leftResolutionArrow->onClick([resolutions, resolutionsIndexPtr, resolutionValue, &btnSound]
		{
			btnSound.play();
			if (*resolutionsIndexPtr == 0) *resolutionsIndexPtr = resolutions->size() - 1;
			else --(*resolutionsIndexPtr);
			resolutionValue->setText(std::to_string((*resolutions)[*resolutionsIndexPtr].x) + "x"
				+ std::to_string((*resolutions)[*resolutionsIndexPtr].y));
		});
	rightResolutionArrow->onClick([resolutions, resolutionsIndexPtr, resolutionValue, &btnSound]
		{

			btnSound.play();
			if (*resolutionsIndexPtr == resolutions->size() - 1) *resolutionsIndexPtr = 0;
			else ++(*resolutionsIndexPtr);
			resolutionValue->setText(std::to_string((*resolutions)[*resolutionsIndexPtr].x) + "x"
				+ std::to_string((*resolutions)[*resolutionsIndexPtr].y));
		});

	resolutionContainer->add(leftResolutionArrow);
	resolutionContainer->add(resolutionValue);
	resolutionContainer->add(rightResolutionArrow);

	auto fullscreenText = tgui::Label::create("Fullscreen");
	fullscreenText->setSize("16%", "12.5%");
	fullscreenText->setPosition("5%", "35%");
	fullscreenText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	fullscreenText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	fullscreenText->getRenderer()->setBorders(2);
	fullscreenText->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
	fullscreenText->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
	fullscreenText->getRenderer()->setTextOutlineThickness(1.5f);
	fullscreenText->setTextSize(24);
	videoPanel->add(fullscreenText);

	auto fullscreenCheckbox = tgui::CheckBox::create();
	fullscreenCheckbox->setSize("5%", "6%");
	fullscreenCheckbox->setPosition("73%", "37.5%");
	fullscreenCheckbox->setChecked(true);
	fullscreenCheckbox->onCheck([fullscreenPtr, &btnSound] {
		btnSound.play();
		*fullscreenPtr = true;
		});
	fullscreenCheckbox->onUncheck([fullscreenPtr, &btnSound] {
		btnSound.play();
		*fullscreenPtr = false;
		});

	auto framerateContainer = tgui::Panel::create();
	framerateContainer->setSize({ "40%", "12.5%" });
	framerateContainer->setPosition("75%", "60%");
	framerateContainer->setOrigin(0.5f, 0.f);
	framerateContainer->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));

	auto applyButton = tgui::Button::create("Apply");
	applyButton->setSize({ "15%", "10%" });
	applyButton->setPosition("75%", "80%");
	applyButton->getRenderer()->setBorders(2);
	applyButton->getRenderer()->setRoundedBorderRadius(10);
	applyButton->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(applyButton);
	applyButton->onClick([resolutionsIndexPtr, fullscreenPtr, framerateIndexPtr,
		masterVolPtr, buttonsVolPtr, bgVolPtr, onApply, &btnSound]
		{
			btnSound.play();
			if (onApply) {
				onApply(
					*resolutionsIndexPtr,
					*fullscreenPtr,
					*framerateIndexPtr,
					*masterVolPtr,
					*buttonsVolPtr,
					*bgVolPtr
				);
			}
		});
	content->add(applyButton);

	auto closeButton = tgui::Button::create("Close");
	closeButton->setSize({ "15%", "10%" });
	closeButton->setPosition("10%", "80%");
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

	auto framerateText = tgui::Label::create("Framerate");
	framerateText->setSize("16%", "12.5%");
	framerateText->setPosition("5%", "60%");
	framerateText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	framerateText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	framerateText->setTextSize(24);
	framerateText->getRenderer()->setBorders(2);
	framerateText->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
	framerateText->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
	framerateText->getRenderer()->setTextOutlineThickness(1.5f);
	videoPanel->add(framerateText);

	auto framerateValue = tgui::Label::create(std::to_string((*framerates)[*framerateIndexPtr]));
	framerateValue->setSize("70%", "100%");
	framerateValue->setPosition("15%", "0%");
	framerateValue->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	framerateValue->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	framerateValue->setTextSize(24);
	framerateValue->getRenderer()->setBorders(2);
	framerateValue->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
	framerateValue->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
	framerateValue->getRenderer()->setTextOutlineThickness(1.5f);

	auto leftFramerateArrow = tgui::Button::create("<");
	leftFramerateArrow->setSize({ "15%", "100%" });
	leftFramerateArrow->setPosition("0%", "0%");
	leftFramerateArrow->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(leftFramerateArrow);
	leftFramerateArrow->onClick([framerateIndexPtr, framerates, framerateValue, &btnSound] {
		btnSound.play();
		if (*framerateIndexPtr == 0) *framerateIndexPtr = (*framerates).size() - 1;
		else --(*framerateIndexPtr);
		framerateValue->setText(std::to_string((*framerates)[*framerateIndexPtr]));
		});

	auto rightFramerateArrow = tgui::Button::create(">");
	rightFramerateArrow->setSize({ "15%", "100%" });
	rightFramerateArrow->setPosition({ "85%", "0%" });
	rightFramerateArrow->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(rightFramerateArrow);
	rightFramerateArrow->onClick([framerateIndexPtr, framerates, framerateValue, &btnSound] {
		btnSound.play();
		if (*framerateIndexPtr == (*framerates).size() - 1) *framerateIndexPtr = 0;
		else ++(*framerateIndexPtr);
		framerateValue->setText(std::to_string((*framerates)[*framerateIndexPtr]));
		});

	framerateContainer->add(framerateValue);
	framerateContainer->add(leftFramerateArrow);
	framerateContainer->add(rightFramerateArrow);

	videoPanel->add(resolutionContainer);
	videoPanel->add(fullscreenCheckbox);
	videoPanel->add(framerateContainer);

	auto createAudioLabel = [](const tgui::String& text) {
		auto label = tgui::Label::create(text);
		label->setSize({ "25%", "12.5%" });
		label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		label->getRenderer()->setBorders(2);
		label->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
		label->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
		label->getRenderer()->setTextOutlineThickness(1.5f);
		label->setTextSize(22);
		return label;
		};

	auto masterLabel = createAudioLabel("Master Volume");
	masterLabel->setPosition({ "5%", "10%" });
	audioPanel->add(masterLabel);

	auto masterSlider = tgui::Slider::create();
	masterSlider->setPosition({ "35%", "13.5%" });
	masterSlider->setSize({ "55%", "2%" });
	masterSlider->setMinimum(0);
	masterSlider->setMaximum(100);
	masterSlider->setValue(*masterVolPtr);
	StyleSlider(masterSlider);
	masterSlider->onValueChange([masterVolPtr](float value) {
		*masterVolPtr = value;
		});
	audioPanel->add(masterSlider);

	auto buttonsLabel = createAudioLabel("Buttons Volume");
	buttonsLabel->setPosition({ "5%", "35%" });
	audioPanel->add(buttonsLabel);

	auto buttonsSlider = tgui::Slider::create();
	buttonsSlider->setPosition({ "35%", "38.5%" });
	buttonsSlider->setSize({ "55%", "2%" });
	buttonsSlider->setMinimum(0);
	buttonsSlider->setMaximum(100);
	buttonsSlider->setValue(*buttonsVolPtr);
	StyleSlider(buttonsSlider);
	buttonsSlider->onValueChange([buttonsVolPtr](float value) {
		*buttonsVolPtr = value;
		});
	audioPanel->add(buttonsSlider);

	auto bgLabel = createAudioLabel("Background Vol.");
	bgLabel->setPosition({ "5%", "60%" });
	audioPanel->add(bgLabel);

	auto bgSlider = tgui::Slider::create();
	bgSlider->setPosition({ "35%", "63.5%" });
	bgSlider->setSize({ "55%", "2%" });
	bgSlider->setMinimum(0);
	bgSlider->setMaximum(100);
	bgSlider->setValue(*bgVolPtr);
	StyleSlider(bgSlider);
	bgSlider->onValueChange([bgVolPtr](float value) {
		*bgVolPtr = value;
		});
	audioPanel->add(bgSlider);

	return panel;
}

tgui::Panel::Ptr Utils::CreateProfilePanel(
	const UserProfile& profile,
	sf::Sound& btnSound,
	std::function<void()> onClose)
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

	auto hoursPlayed = tgui::Label::create("Hours played: " + std::to_string(profile.hoursPlayed));
	hoursPlayed->setPosition({ "5%", "58%" });
	hoursPlayed->getRenderer()->setTextSize(24);
	panel->add(hoursPlayed);

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

tgui::Panel::Ptr Utils::CreateExitPanel(tgui::Gui& gui,
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

tgui::Panel::Ptr Utils::CreateErrorPopupPanel(tgui::Gui& gui,
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

void Utils::StyleButton(const tgui::Button::Ptr& button)
{
	button->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
	button->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
	button->getRenderer()->setTextOutlineThickness(1.5f);
	button->getRenderer()->setOpacity(0.7f);
	button->getRenderer()->setTextSize(24);
}

void Utils::StyleSlider(const tgui::Slider::Ptr& slider)
{
	auto renderer = slider->getRenderer();
	renderer->setTrackColor(tgui::Color::Black);
	renderer->setTrackColorHover(tgui::Color::Black);
	renderer->setBorders(0);
	renderer->setThumbColor(tgui::Color(245, 240, 230));
	renderer->setThumbColorHover(tgui::Color::White);
}
