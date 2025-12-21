#include "Utils.h"

tgui::String Utils::TruncateText(const tgui::String& text, unsigned maxChars)
{
    if (text.length() < maxChars)
    {
        return text;
    }

    return text.substr(0, maxChars - 1) + "...";
}

tgui::Panel::Ptr Utils::CreateSettingsPanel(tgui::Gui& gui,
	unsigned resolutionIndex,
	bool isFullscreen,
	unsigned framerateIndex,
	std::function<void()> onClose,
	std::function<void(unsigned res, bool fullscreen, unsigned framerate)> onApply)
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

	auto panel = tgui::Panel::create({ "60%", "60%" });
	panel->setPosition({ "50%", "50%" });
	panel->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 180));
	panel->getRenderer()->setBorders({ 2 });
	panel->getRenderer()->setBorderColor(tgui::Color::Black);
	panel->getRenderer()->setRoundedBorderRadius(15);
	panel->setOrigin(0.5f, 0.5f);

	auto background = tgui::Picture::create("assets/images/mainMenuBackground.png");
	background->setSize({ "100%", "100%" });

	auto sidebar = tgui::Panel::create({ "20%", "100%" });
	sidebar->setPosition({ "0%", "0%" });
	sidebar->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50, 180));
	sidebar->getRenderer()->setBorders({ 1 });
	sidebar->getRenderer()->setBorderColor(tgui::Color::White);
	sidebar->add(background);
	panel->add(sidebar);

	auto content = tgui::Panel::create({ "80%", "100%" });
	content->setPosition({ "20%", "0%" });
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

	videoPanel->setPosition({ "0%", "0%" });
	videoPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	videoPanel->add(background);

	audioPanel->setPosition({ "0%", "0%" });
	audioPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	audioPanel->add(background);

	gameplayPanel->setPosition({ "0%", "0%" });
	gameplayPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	gameplayPanel->add(background);

	auto videoButton = tgui::Button::create();
	videoButton->setSize({ "90%", "10%" });
	videoButton->setPosition({ "5%", "20%" });
	videoButton->setText("Video");
	videoButton->getRenderer()->setTextColor(tgui::Color::Red);
	videoButton->getRenderer()->setBorderColor(tgui::Color::White);
	videoButton->getRenderer()->setBorders({ 1 });
	videoButton->setTextSize(24);
	videoButton->getRenderer()->setTexture("assets/images/buttons.png");
	sidebar->add(videoButton);

	auto audioButton = tgui::Button::create();
	audioButton->setSize({ "90%", "10%" });
	audioButton->setPosition({ "5%", "35%" });
	audioButton->setText("Audio");
	audioButton->getRenderer()->setTextColor(tgui::Color::Red);
	audioButton->getRenderer()->setBorderColor(tgui::Color::White);
	audioButton->getRenderer()->setBorders({ 1 });
	audioButton->setTextSize(24);
	audioButton->getRenderer()->setTexture("assets/images/buttons.png");
	sidebar->add(audioButton);

	auto gameplayButton = tgui::Button::create();
	gameplayButton->setSize({ "90%", "10%" });
	gameplayButton->setPosition({ "5%", "50%" });
	gameplayButton->setText("Gameplay");
	gameplayButton->getRenderer()->setTextColor(tgui::Color::Red);
	gameplayButton->getRenderer()->setBorderColor(tgui::Color::White);
	gameplayButton->getRenderer()->setBorders({ 1 });
	gameplayButton->setTextSize(24);
	gameplayButton->getRenderer()->setTexture("assets/images/buttons.png");
	sidebar->add(gameplayButton);

	videoButton->onClick([=] {
		videoPanel->setVisible(true);
		audioPanel->setVisible(false);
		gameplayPanel->setVisible(false);
		});

	audioButton->onClick([=] {
		videoPanel->setVisible(false);
		audioPanel->setVisible(true);
		gameplayPanel->setVisible(false);
		});

	gameplayButton->onClick([=] {
		videoPanel->setVisible(false);
		audioPanel->setVisible(false);
		gameplayPanel->setVisible(true);
		});

	auto resolutionContainer = tgui::Panel::create();
	resolutionContainer->setSize({ "40%", "12.5%" });
	resolutionContainer->setPosition({ "75%", "10%" });
	resolutionContainer->setOrigin(0.5f, 0.f);
	resolutionContainer->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));

	auto resolutionText = tgui::Label::create("Resolution");
	resolutionText->setSize({ "16%", "12.5%" });
	resolutionText->setPosition({ "5%", "10%" });
	resolutionText->getRenderer()->setTextColor(tgui::Color::Red);
	resolutionText->getRenderer()->setBorders({ 2 });
	resolutionText->getRenderer()->setBorderColor(tgui::Color::Black);
	resolutionText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	resolutionText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	resolutionText->setTextSize(24);
	videoPanel->add(resolutionText);

	auto resolutionValue = tgui::Label::create(
		std::to_string((*resolutions)[*resolutionsIndexPtr].x) + "x" +
		std::to_string((*resolutions)[*resolutionsIndexPtr].y)
	);
	resolutionValue->setSize({ "70%", "100%" });
	resolutionValue->setPosition({ "15%", "0%" });
	resolutionValue->getRenderer()->setTextColor(tgui::Color::Red);
	resolutionValue->getRenderer()->setBorders({ 2 });
	resolutionValue->getRenderer()->setBorderColor(tgui::Color::Black);
	resolutionValue->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	resolutionValue->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	resolutionValue->setTextSize(24);

	auto leftResolutionArrow = tgui::Button::create("<");
	leftResolutionArrow->setSize({ "15%", "100%" });
	leftResolutionArrow->setPosition({ "0%", "0%" });
	leftResolutionArrow->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	leftResolutionArrow->getRenderer()->setTextColor(tgui::Color::Black);
	leftResolutionArrow->getRenderer()->setTexture("assets/images/buttons.png");
	leftResolutionArrow->setTextSize(24);

	auto rightResolutionArrow = tgui::Button::create(">");
	rightResolutionArrow->setSize({ "15%", "100%" });
	rightResolutionArrow->setPosition({ "85%", "0%" });
	rightResolutionArrow->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	rightResolutionArrow->getRenderer()->setTextColor(tgui::Color::Black);
	rightResolutionArrow->getRenderer()->setTexture("assets/images/buttons.png");
	rightResolutionArrow->setTextSize(24);

	leftResolutionArrow->onClick([resolutions, resolutionsIndexPtr, resolutionValue]
		{
			if (*resolutionsIndexPtr == 0) *resolutionsIndexPtr = resolutions->size() - 1;
			else --(*resolutionsIndexPtr);
			resolutionValue->setText(std::to_string((*resolutions)[*resolutionsIndexPtr].x) + "x"
				+ std::to_string((*resolutions)[*resolutionsIndexPtr].y));
		});
	rightResolutionArrow->onClick([resolutions, resolutionsIndexPtr, resolutionValue]
		{
			if (*resolutionsIndexPtr == resolutions->size() - 1) *resolutionsIndexPtr = 0;
			else ++(*resolutionsIndexPtr);
			resolutionValue->setText(std::to_string((*resolutions)[*resolutionsIndexPtr].x) + "x"
				+ std::to_string((*resolutions)[*resolutionsIndexPtr].y));
		});

	resolutionContainer->add(leftResolutionArrow);
	resolutionContainer->add(resolutionValue);
	resolutionContainer->add(rightResolutionArrow);

	auto fullscreenText = tgui::Label::create("Fullscreen");
	fullscreenText->setSize({ "16%", "12.5%" });
	fullscreenText->setPosition({ "5%", "35%" });
	fullscreenText->getRenderer()->setTextColor(tgui::Color::Red);
	fullscreenText->getRenderer()->setBorders({ 2 });
	fullscreenText->getRenderer()->setBorderColor(tgui::Color::Black);
	fullscreenText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	fullscreenText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	fullscreenText->setTextSize(24);
	videoPanel->add(fullscreenText);

	auto fullscreenCheckbox = tgui::CheckBox::create();
	fullscreenCheckbox->setSize({ "5%", "6%" });
	fullscreenCheckbox->setPosition({ "73%", "37.5%" });
	fullscreenCheckbox->setChecked(true);
	fullscreenCheckbox->onCheck([fullscreenPtr] {
		*fullscreenPtr = true;
		});
	fullscreenCheckbox->onUncheck([fullscreenPtr] {
		*fullscreenPtr = false;
		});

	auto framerateContainer = tgui::Panel::create();
	framerateContainer->setSize({ "40%", "12.5%" });
	framerateContainer->setPosition({ "75%", "60%" });
	framerateContainer->setOrigin(0.5f, 0.f);
	framerateContainer->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));

	auto applyButton = tgui::Button::create("Apply");
	applyButton->setSize({ "15%", "10%" });
	applyButton->setPosition({ "75%", "80%" });
	applyButton->setTextSize(24);
	applyButton->getRenderer()->setBackgroundColor(tgui::Color("#f0f0f0"));
	applyButton->getRenderer()->setBackgroundColorHover(tgui::Color("#ffffff"));
	applyButton->getRenderer()->setBorders({ 2 });
	applyButton->getRenderer()->setBorderColor(tgui::Color::Black);
	applyButton->getRenderer()->setTextColor(tgui::Color::Black);
	applyButton->getRenderer()->setRoundedBorderRadius(10);
	applyButton->getRenderer()->setTexture("assets/images/buttons.png");
	applyButton->onClick([resolutionsIndexPtr, fullscreenPtr,
		framerateIndexPtr, onApply]
		{
			if (onApply) onApply(*resolutionsIndexPtr, *fullscreenPtr, *framerateIndexPtr);
		});
	content->add(applyButton);

	auto closeButton = tgui::Button::create("Close");
	closeButton->setSize({ "15%", "10%" });
	closeButton->setPosition({ "10%", "80%" });
	closeButton->setTextSize(24);
	closeButton->getRenderer()->setBackgroundColor(tgui::Color("#f0f0f0"));
	closeButton->getRenderer()->setBackgroundColorHover(tgui::Color("#ffffff"));
	closeButton->getRenderer()->setBorders({ 2 });
	closeButton->getRenderer()->setBorderColor(tgui::Color::Black);
	closeButton->getRenderer()->setTextColor(tgui::Color::Black);
	closeButton->getRenderer()->setRoundedBorderRadius(10);
	closeButton->getRenderer()->setTexture("assets/images/buttons.png");
	closeButton->onClick([panel, onClose] {
		panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
		if (onClose) onClose();
		});
	content->add(closeButton);

	auto framerateText = tgui::Label::create("Framerate");
	framerateText->setSize({ "16%", "12.5%" });
	framerateText->setPosition({ "5%", "60%" });
	framerateText->getRenderer()->setTextColor(tgui::Color::Red);
	framerateText->getRenderer()->setBorders({ 2 });
	framerateText->getRenderer()->setBorderColor(tgui::Color::Black);
	framerateText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	framerateText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	framerateText->setTextSize(24);
	videoPanel->add(framerateText);

	auto framerateValue = tgui::Label::create(std::to_string((*framerates)[*framerateIndexPtr]));
	framerateValue->setSize({ "70%", "100%" });
	framerateValue->setPosition({ "15%", "0%" });
	framerateValue->getRenderer()->setTextColor(tgui::Color::Red);
	framerateValue->getRenderer()->setBorders({ 2 });
	framerateValue->getRenderer()->setBorderColor(tgui::Color::Black);
	framerateValue->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	framerateValue->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	framerateValue->setTextSize(24);

	auto leftFramerateArrow = tgui::Button::create("<");
	leftFramerateArrow->setSize({ "15%", "100%" });
	leftFramerateArrow->setPosition({ "0%", "0%" });
	leftFramerateArrow->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	leftFramerateArrow->getRenderer()->setTextColor(tgui::Color::Black);
	leftFramerateArrow->getRenderer()->setTexture("assets/images/buttons.png");
	leftFramerateArrow->setTextSize(24);
	leftFramerateArrow->onClick([framerateIndexPtr, framerates, framerateValue] {
		if (*framerateIndexPtr == 0) *framerateIndexPtr = (*framerates).size() - 1;
		else --(*framerateIndexPtr);
		framerateValue->setText(std::to_string((*framerates)[*framerateIndexPtr]));
		});

	auto rightFramerateArrow = tgui::Button::create(">");
	rightFramerateArrow->setSize({ "15%", "100%" });
	rightFramerateArrow->setPosition({ "85%", "0%" });
	rightFramerateArrow->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	rightFramerateArrow->getRenderer()->setTextColor(tgui::Color::Black);
	rightFramerateArrow->getRenderer()->setTexture("assets/images/buttons.png");
	rightFramerateArrow->setTextSize(24);
	rightFramerateArrow->onClick([framerateIndexPtr, framerates, framerateValue] {
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

	return panel;
}

tgui::Panel::Ptr Utils::CreateExitPanel(tgui::Gui& gui,
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
	textLabel->setPosition({ "50%", "35%" });
	textLabel->setOrigin(0.5f, 0.5f);
	textLabel->setTextSize(24);

	auto noButton = tgui::Button::create("No");
	noButton->setSize({ "16.67%", "20%" });
	noButton->setPosition({ "10%", "70%" });
	noButton->getRenderer()->setTexture("assets/images/buttons.png");
	noButton->getRenderer()->setTextColor(tgui::Color::White);
	noButton->setTextSize(20);
	noButton->onPress([panel, onNo]() {
		panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(100));
		if (onNo) onNo();
		});

	auto yesButton = tgui::Button::create("Yes");
	yesButton->setSize({ "16.67%", "20%" });
	yesButton->setPosition({ "75%", "70%" });
	yesButton->getRenderer()->setTexture("assets/images/buttons.png");
	yesButton->getRenderer()->setTextColor(tgui::Color::White);
	yesButton->setTextSize(20);
	yesButton->onPress([panel, onYes]() {
		if (onYes) onYes();
		});

	panel->add(textLabel);
	panel->add(noButton);
	panel->add(yesButton);

	return panel;
}

tgui::Panel::Ptr Utils::CreateErrorPopupPanel(tgui::Gui& gui, const std::string& message, std::function<void()> onClose)
{

	auto errorPopupPanel = tgui::Panel::create({ "25%", "25%" });
	errorPopupPanel->setPosition({ "50%", "50%" });
	errorPopupPanel->setOrigin(0.5f, 0.5f);
	errorPopupPanel->getRenderer()->setBorderColor(tgui::Color::Black);
	errorPopupPanel->getRenderer()->setBorders(1);

	auto background = tgui::Picture::create("assets/images/mainMenuBackground.png");
	background->setSize({ "100%", "100%" });
	errorPopupPanel->add(background);

	auto okButton = tgui::Button::create("Ok");
	okButton->setSize({ "25%", "25%" });
	okButton->setPosition({ "50%", "75%" });
	okButton->setTextSize(24);
	okButton->setOrigin(0.5f, 0.5f);
	okButton->getRenderer()->setTexture("assets/images/buttons.png");
	okButton->getRenderer()->setBorderColor(tgui::Color::Black);
	okButton->getRenderer()->setBorders(1);
	okButton->onPress([errorPopupPanel, onClose]()
		{
			errorPopupPanel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
			if (onClose) onClose();
		});
	errorPopupPanel->add(okButton);

	auto errorLabel = tgui::Label::create(message);
	errorLabel->setSize({ "60%", "40%" });
	errorLabel->setPosition({ "50%", "45%" });
	errorLabel->setTextSize(24);
	errorLabel->setOrigin(0.5f, 0.5f);
	errorLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	errorLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);

	errorPopupPanel->add(errorLabel);

	return errorPopupPanel;
}
