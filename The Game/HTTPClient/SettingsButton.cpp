#include "SettingsButton.h"

SettingsButton::SettingsButton(sf::RenderWindow& window) :
	m_settingsButton(window)
{
	LoadWidgets();
}

void SettingsButton::Draw()
{
	m_settingsButton.draw();
}

void SettingsButton::HandleEvent(sf::Event event)
{
	m_settingsButton.handleEvent(event);
}

void SettingsButton::SetTarget(sf::RenderWindow& window)
{
	m_settingsButton.setTarget(window);
}

void SettingsButton::CloseButtonPressed()
{
	m_closePressed = true;
}

bool SettingsButton::GetCloseState() const
{
	return m_closePressed;
}

bool SettingsButton::GetApplyState() const
{
	return m_applyPressed;
}

const sf::Vector2u& SettingsButton::GetCurrentResolution() const
{
	return m_resolutions[m_resolutionIndex];
}

void SettingsButton::SetCloseState()
{
	m_closePressed = false;
}

void SettingsButton::SetApplyState()
{
	m_applyPressed = false;
}

void SettingsButton::LoadWidgets()
{
	auto content = tgui::Panel::create();
	content->setSize({ "80%", "100%" });
	content->setPosition({ "20%", "0%" });
	content->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));

	auto videoPanel = tgui::Panel::create();
	videoPanel->setSize({ "100%", "100%" });
	videoPanel->setPosition({ "0%", "0%" });
	videoPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));

	auto audioPanel = tgui::Panel::create();
	audioPanel->setSize({ "100%", "100%" });
	audioPanel->setPosition({ "0%", "0%" });
	audioPanel->setVisible(false);
	audioPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));

	auto gameplayPanel = tgui::Panel::create();
	gameplayPanel->setSize({ "100%", "100%" });
	gameplayPanel->setPosition({ "0%", "0%" });
	gameplayPanel->setVisible(false);
	gameplayPanel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));

	m_panel = tgui::Panel::create();
	auto panel = m_panel;
	panel->setSize({ "60%", "60%" });
	panel->setPosition({ "50%", "50%" });
	panel->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 180));
	panel->getRenderer()->setBorders({ 2 });
	panel->getRenderer()->setBorderColor(tgui::Color::White);
	panel->getRenderer()->setRoundedBorderRadius(15);
	panel->setOrigin(0.5f, 0.5f);

	auto sidebar = tgui::Panel::create();
	sidebar->setSize({ "20%", "100%" });
	sidebar->setPosition({ "0%", "0%" });
	sidebar->getRenderer()->setBackgroundColor(tgui::Color(50, 50, 50, 180));
	sidebar->getRenderer()->setBorders({ 1 });
	sidebar->getRenderer()->setBorderColor(tgui::Color::White);

	auto videoButton = tgui::Button::create();
	videoButton->setSize({ "90%", "10%" });
	videoButton->setPosition({ "5%", "20%" });
	videoButton->setText("Video");
	videoButton->getRenderer()->setTextColor(tgui::Color::Red);
	videoButton->getRenderer()->setBorderColor(tgui::Color::White);
	videoButton->getRenderer()->setBorders({ 1 });
	videoButton->setTextSize(24);

	auto audioButton = tgui::Button::create();
	audioButton->setSize({ "90%", "10%" });
	audioButton->setPosition({ "5%", "35%" });
	audioButton->setText("Audio");
	audioButton->getRenderer()->setTextColor(tgui::Color::Red);
	audioButton->getRenderer()->setBorderColor(tgui::Color::White);
	audioButton->getRenderer()->setBorders({ 1 });
	audioButton->setTextSize(24);

	auto gameplayButton = tgui::Button::create();
	gameplayButton->setSize({ "90%", "10%" });
	gameplayButton->setPosition({ "5%", "50%" });
	gameplayButton->setText("Gameplay");
	gameplayButton->getRenderer()->setTextColor(tgui::Color::Red);
	gameplayButton->getRenderer()->setBorderColor(tgui::Color::White);
	gameplayButton->getRenderer()->setBorders({ 1 });
	gameplayButton->setTextSize(24);

	auto closeButton = tgui::Button::create();
	closeButton->setSize({ "15%", "10%" });
	closeButton->setPosition({ "10%", "80%" });
	closeButton->setText("Close");
	closeButton->setTextSize(24);
	closeButton->getRenderer()->setBackgroundColor(tgui::Color("#f0f0f0"));
	closeButton->getRenderer()->setBackgroundColorHover(tgui::Color("#ffffff"));
	closeButton->getRenderer()->setBorders({ 2 });
	closeButton->getRenderer()->setBorderColor(tgui::Color::Black);
	closeButton->getRenderer()->setTextColor(tgui::Color::Black);
	closeButton->getRenderer()->setRoundedBorderRadius(10);

	auto applyButton = tgui::Button::create();
	applyButton->setSize({ "15%", "10%" });
	applyButton->setPosition({ "75%", "80%" });
	applyButton->setText("Apply");
	applyButton->setTextSize(24);
	applyButton->getRenderer()->setBackgroundColor(tgui::Color("#f0f0f0"));
	applyButton->getRenderer()->setBackgroundColorHover(tgui::Color("#ffffff"));
	applyButton->getRenderer()->setBorders({ 2 });
	applyButton->getRenderer()->setBorderColor(tgui::Color::Black);
	applyButton->getRenderer()->setTextColor(tgui::Color::Black);
	applyButton->getRenderer()->setRoundedBorderRadius(10);
	applyButton->onClick([this]
		{
			m_applyPressed = true;
		});

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

	auto resolutionContainer = tgui::Panel::create();
	resolutionContainer->setSize({ "40%", "12.5%" });
	resolutionContainer->setPosition({ "75%", "10%" });
	resolutionContainer->setOrigin(0.5f, 0.f);
	resolutionContainer->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));

	auto resolutionValue = tgui::Label::create();
	resolutionValue->setSize({ "70%", "100%" });
	resolutionValue->setPosition({ "15%", "0%" });
	resolutionValue->getRenderer()->setTextColor(tgui::Color::Red);
	resolutionValue->getRenderer()->setBorders({ 2 });
	resolutionValue->getRenderer()->setBorderColor(tgui::Color::Black);
	resolutionValue->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	resolutionValue->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	resolutionValue->setText(std::to_string(m_resolutions[m_resolutionIndex].x) + "x"
		+ std::to_string(m_resolutions[m_resolutionIndex].y));
	resolutionValue->setTextSize(24);

	auto leftArrow = tgui::Button::create("<");
	leftArrow->setSize({ "15%", "100%" });
	leftArrow->setPosition({"0%", "0%"});
	leftArrow->setTextSize(24);
	leftArrow->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	leftArrow->getRenderer()->setTextColor(tgui::Color::Black);
	leftArrow->onClick([this, resolutionValue]
		{
			if (m_resolutionIndex > 0) --m_resolutionIndex;
			else
			{
				m_resolutionIndex = m_resolutions.size() - 1;
			}
			resolutionValue->setText(std::to_string(m_resolutions[m_resolutionIndex].x)
				+ "x" + std::to_string(m_resolutions[m_resolutionIndex].y));
		});

	auto rightArrow = tgui::Button::create(">");
	rightArrow->setSize({ "15%", "100%" });
	rightArrow->setPosition({ "85%", "0%" });
	rightArrow->setTextSize(24);
	rightArrow->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	rightArrow->getRenderer()->setTextColor(tgui::Color::Black);
	rightArrow->onClick([this, resolutionValue]
		{
			if (m_resolutionIndex < m_resolutions.size() - 1) ++m_resolutionIndex;
			else
			{
				m_resolutionIndex = 0;
			}
			resolutionValue->setText(std::to_string(m_resolutions[m_resolutionIndex].x)
				+ "x" + std::to_string(m_resolutions[m_resolutionIndex].y));
		});

	resolutionContainer->add(leftArrow);
	resolutionContainer->add(resolutionValue);
	resolutionContainer->add(rightArrow);

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

	videoPanel->add(resolutionText);
	videoPanel->add(resolutionContainer);
	videoPanel->add(fullscreenText);
	videoPanel->add(framerateText);

	sidebar->add(videoButton);
	sidebar->add(audioButton);
	sidebar->add(gameplayButton);

	content->add(videoPanel);
	content->add(audioPanel);
	content->add(gameplayPanel);
	content->add(closeButton);
	content->add(applyButton);

	panel->add(sidebar);
	panel->add(content);

	closeButton->onClick([this] {CloseButtonPressed(); });
	videoButton->onClick([videoPanel, audioPanel, gameplayPanel] {
		videoPanel->setVisible(true);
		audioPanel->setVisible(false);
		gameplayPanel->setVisible(false);
		});
	audioButton->onClick([videoPanel, audioPanel, gameplayPanel] {
		videoPanel->setVisible(false);
		audioPanel->setVisible(true);
		gameplayPanel->setVisible(true);
		});
	gameplayButton->onClick([videoPanel, audioPanel, gameplayPanel] {
		videoPanel->setVisible(false);
		audioPanel->setVisible(false);
		gameplayPanel->setVisible(true);
		});

	m_settingsButton.add(panel);
}
