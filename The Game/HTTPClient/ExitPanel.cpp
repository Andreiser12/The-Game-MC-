#include "ExitPanel.h"

tgui::Panel::Ptr ExitPanel::Create(tgui::Gui& gui,
    std::function<void()> onYes,
    std::function<void()> onNo)
{
    auto panel = tgui::Panel::create({ "33.33%", "33.33%" });
    panel->setPosition({ "50%", "50%" });
    panel->setOrigin(0.5f, 0.5f);

    auto background = tgui::Picture::create("assets/images/background.png");
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
