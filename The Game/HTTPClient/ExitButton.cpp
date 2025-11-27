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

void ExitButton::NoPressed()
{
    m_noPressed = true;
}

void ExitButton::YesPressed()
{
    m_yesPressed = true;
}

void ExitButton::SetNoState()
{
    m_noPressed = false;
}

void ExitButton::SetYesState()
{
    m_yesPressed = false;
}

bool ExitButton::GetNoState() const
{
    return m_noPressed;
}

bool ExitButton::GetYesState() const
{
    return m_yesPressed;
}

tgui::Panel::Ptr ExitButton::GetPanel() const
{
    return m_panel;
}

void ExitButton::LoadWidgets()
{
    m_panel = tgui::Panel::create();
    auto panel = m_panel;
    panel->setSize({ "33.33%", "33.33%" });
    panel->getRenderer()->setBackgroundColor(tgui::Color::Black);
    panel->setPosition({ "50%", "50%" });
    panel->setOrigin(0.5f, 0.5f);

    auto textLabel = tgui::Label::create();
    textLabel->setSize({ "60%", "12.5%" });
    textLabel->setPosition({ "50%", "40%" });
    textLabel->setOrigin(0.5f, 0.5f);
    textLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    textLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    textLabel->getRenderer()->setTextColor(tgui::Color::White);
    textLabel->setText("Are you sure you want to exit?");
    textLabel->setTextSize(24);

    auto noButton = tgui::Button::create();
    noButton->setSize({ "16.67%", "20%" });
    noButton->setPosition({ "10%", "70%" });
    noButton->getRenderer()->setBackgroundColor(tgui::Color::Red);
    noButton->getRenderer()->setTextColor(tgui::Color::White);
    noButton->setText("No");
    noButton->setTextSize(20);
    noButton->onClick([this] { NoPressed(); });

    auto yesButton = tgui::Button::create();
    yesButton->setSize({ "16.67%", "20%" });
    yesButton->setPosition({ "75%", "70%" });
    yesButton->getRenderer()->setBackgroundColor(tgui::Color::Green);
    yesButton->getRenderer()->setTextColor(tgui::Color::White);
    yesButton->setText("Yes");
    yesButton->setTextSize(20);
    yesButton->onClick([this] { YesPressed(); });


    panel->add(textLabel);
    panel->add(noButton);
    panel->add(yesButton);

    m_exitButton.add(panel);

}
