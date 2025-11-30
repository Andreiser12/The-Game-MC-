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

void SettingsButton::CloseButtonPressed()
{
    m_closePressed = true;
}

bool SettingsButton::GetCloseState() const
{
    return m_closePressed;
}

void SettingsButton::SetCloseState()
{
    m_closePressed = false;
}

void SettingsButton::LoadWidgets()
{
    m_panel = tgui::Panel::create();
    auto panel = m_panel;
    panel->setSize({ "33.33%", "33.33%" });
    panel->setPosition({ "50%", "50%" });
    panel->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 180));
    panel->getRenderer()->setBorders({ 2 });
    panel->getRenderer()->setBorderColor(tgui::Color::White);
    panel->getRenderer()->setRoundedBorderRadius(15);
    panel->setOrigin(0.5f, 0.5f);
    panel->getRenderer()->setBackgroundColor(
        tgui::Color::applyOpacity(tgui::Color::Black, 0.6f));

    auto closeButton = tgui::Button::create();
    closeButton->setSize({ "16.67%", "10%" });
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
    applyButton->setSize({ "16.67%", "10%" });
    applyButton->setPosition({ "75%", "80%" });
    applyButton->setText("Apply");
    applyButton->setTextSize(24);
    applyButton->getRenderer()->setBackgroundColor(tgui::Color("#f0f0f0"));
    applyButton->getRenderer()->setBackgroundColorHover(tgui::Color("#ffffff"));
    applyButton->getRenderer()->setBorders({ 2 });
    applyButton->getRenderer()->setBorderColor(tgui::Color::Black);
    applyButton->getRenderer()->setTextColor(tgui::Color::Black);
    applyButton->getRenderer()->setRoundedBorderRadius(10);

    panel->add(closeButton);
    closeButton->onClick([this] {CloseButtonPressed(); });
    panel->add(applyButton);

    m_settingsButton.add(panel);
}
