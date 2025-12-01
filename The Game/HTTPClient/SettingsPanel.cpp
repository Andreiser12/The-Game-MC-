#include "SettingsPanel.h"

tgui::Panel::Ptr SettingsPanel::Create(tgui::Gui& gui,
	std::function<void()> onClose,
	std::function<void(const sf::Vector2u& newResolution)> onApply)
{
	auto panel = tgui::Panel::create({ "60%", "60%" });
	panel->setPosition({ "50%", "50%" });
	panel->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 180));
	panel->getRenderer()->setBorders({ 2 });
	panel->getRenderer()->setBorderColor(tgui::Color::White);
	panel->getRenderer()->setRoundedBorderRadius(15);
	panel->setOrigin(0.5f, 0.5f);

	auto sidebar = tgui::Panel::create({ "20%", "100%" });
	sidebar->setPosition({ "0%", "0%" });
	sidebar->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50, 180));
	sidebar->getRenderer()->setBorders({ 1 });
	sidebar->getRenderer()->setBorderColor(tgui::Color::White);
	panel->add(sidebar);	

	auto content = tgui::Panel::create({ "80%", "100%" });
	content->setPosition({ "20%", "0%" });
	content->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
	panel->add(content);

	auto videoPanel = tgui::Panel::create({"100%", "100%"});
	auto audioPanel = tgui::Panel::create({ "100%", "100%" });
	auto gameplayPanel = tgui::Panel::create({ "100%", "100%" });
	audioPanel->setVisible(false);
	gameplayPanel->setVisible(false);

	content->add(videoPanel);
	content->add(audioPanel);
	content->add(gameplayPanel);

	videoPanel->setPosition({ "0%", "0%" });
	videoPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));

	audioPanel->setPosition({ "0%", "0%" });
	audioPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));

	gameplayPanel->setPosition({ "0%", "0%" });
	gameplayPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));

	auto videoButton = tgui::Button::create();
	videoButton->setSize({ "90%", "10%" });
	videoButton->setPosition({ "5%", "20%" });
	videoButton->setText("Video");
	videoButton->getRenderer()->setTextColor(tgui::Color::Red);
	videoButton->getRenderer()->setBorderColor(tgui::Color::White);
	videoButton->getRenderer()->setBorders({ 1 });
	videoButton->setTextSize(24);
	sidebar->add(videoButton);

	auto audioButton = tgui::Button::create();
	audioButton->setSize({ "90%", "10%" });
	audioButton->setPosition({ "5%", "35%" });
	audioButton->setText("Audio");
	audioButton->getRenderer()->setTextColor(tgui::Color::Red);
	audioButton->getRenderer()->setBorderColor(tgui::Color::White);
	audioButton->getRenderer()->setBorders({ 1 });
	audioButton->setTextSize(24);
	sidebar->add(audioButton);

	auto gameplayButton = tgui::Button::create();
	gameplayButton->setSize({ "90%", "10%" });
	gameplayButton->setPosition({ "5%", "50%" });
	gameplayButton->setText("Gameplay");
	gameplayButton->getRenderer()->setTextColor(tgui::Color::Red);
	gameplayButton->getRenderer()->setBorderColor(tgui::Color::White);
	gameplayButton->getRenderer()->setBorders({ 1 });
	gameplayButton->setTextSize(24);
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
	auto resolutionIndex = std::make_shared<std::size_t>(8u);

	auto resolutionText = tgui::Label::create();
	resolutionText->setSize({ "16%", "12.5%" });
	resolutionText->setPosition({ "5%", "10%" });
	resolutionText->getRenderer()->setTextColor(tgui::Color::Red);
	resolutionText->getRenderer()->setBorders({ 2 });
	resolutionText->getRenderer()->setBorderColor(tgui::Color::Black);
	resolutionText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	resolutionText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	resolutionText->setText("Resolution");
	resolutionText->setTextSize(24);
	videoPanel->add(resolutionText);

	auto resolutionValue = tgui::Label::create(
		std::to_string((*resolutions)[*resolutionIndex].x) + "x" +
		std::to_string((*resolutions)[*resolutionIndex].y)
	);
	resolutionValue->setSize({ "70%", "100%" });
	resolutionValue->setPosition({ "15%", "0%" });
	resolutionValue->getRenderer()->setTextColor(tgui::Color::Red);
	resolutionValue->getRenderer()->setBorders({ 2 });
	resolutionValue->getRenderer()->setBorderColor(tgui::Color::Black);
	resolutionValue->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	resolutionValue->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	resolutionValue->setTextSize(24);

	auto leftArrow = tgui::Button::create("<");
	leftArrow->setSize({ "15%", "100%" });
	leftArrow->setPosition({ "0%", "0%" });
	leftArrow->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	leftArrow->getRenderer()->setTextColor(tgui::Color::Black);
	leftArrow->setTextSize(24);

	auto rightArrow = tgui::Button::create(">");
	rightArrow->setSize({ "15%", "100%" });
	rightArrow->setPosition({ "85%", "0%" });
	rightArrow->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	rightArrow->getRenderer()->setTextColor(tgui::Color::Black);
	rightArrow->setTextSize(24);

	leftArrow->onClick([resolutions, resolutionIndex, resolutionValue]
		{
			if (*resolutionIndex == 0) *resolutionIndex = resolutions->size() - 1;
			else --(*resolutionIndex);
			resolutionValue->setText(std::to_string((*resolutions)[*resolutionIndex].x) + "x"
				+ std::to_string((*resolutions)[*resolutionIndex].y));
		});
	rightArrow->onClick([resolutions, resolutionIndex, resolutionValue]
		{
			if (*resolutionIndex == resolutions->size() - 1) *resolutionIndex = 0;
			else ++(*resolutionIndex);
			resolutionValue->setText(std::to_string((*resolutions)[*resolutionIndex].x) + "x"
				+ std::to_string((*resolutions)[*resolutionIndex].y));
		});

	resolutionContainer->add(leftArrow);
	resolutionContainer->add(resolutionValue);
	resolutionContainer->add(rightArrow);

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
	applyButton->onClick([resolutions, resolutionIndex, onApply]
		{
			if (onApply) onApply((*resolutions)[*resolutionIndex]);
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
	closeButton->onClick([panel, onClose] {
		panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
		if (onClose) onClose();
		});
	content->add(closeButton);

	auto fullscreenText = tgui::Label::create();
	fullscreenText->setSize({ "16%", "12.5%" });
	fullscreenText->setPosition({ "5%", "35%" });
	fullscreenText->getRenderer()->setTextColor(tgui::Color::Red);
	fullscreenText->getRenderer()->setBorders({ 2 });
	fullscreenText->getRenderer()->setBorderColor(tgui::Color::Black);
	fullscreenText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	fullscreenText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	fullscreenText->setText("Fullscreen");
	fullscreenText->setTextSize(24);

	auto framerateText = tgui::Label::create();
	framerateText->setSize({ "16%", "12.5%" });
	framerateText->setPosition({ "5%", "60%" });
	framerateText->getRenderer()->setTextColor(tgui::Color::Red);
	framerateText->getRenderer()->setBorders({ 2 });
	framerateText->getRenderer()->setBorderColor(tgui::Color::Black);
	framerateText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	framerateText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	framerateText->setText("Framerate");
	framerateText->setTextSize(24);

	videoPanel->add(resolutionContainer);
	videoPanel->add(fullscreenText);
	videoPanel->add(framerateText);

	return panel;
}
