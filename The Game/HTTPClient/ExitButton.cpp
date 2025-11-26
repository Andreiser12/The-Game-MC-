#include "ExitButton.h"

ExitButton::ExitButton(sf::RenderWindow& window) :
    m_exitButton(window)
{
    LoadWidgets();
}

void ExitButton::Draw()
{
    m_exitButton.draw();
}

void ExitButton::HandleEvent(sf::Event event)
{
    m_exitButton.handleEvent(event);
}

void ExitButton::UpdateTextSize()
{
}

void ExitButton::LoadWidgets()
{
    auto panel = tgui::Panel::create();
    panel->setSize({ "33.33%", "33.33%" });
    panel->getRenderer()->setBackgroundColor(tgui::Color::Black);
    panel->setPosition({ "50%", "50%" });
    panel->setOrigin(0.5f, 0.5f);
    m_exitButton.add(panel);

}
