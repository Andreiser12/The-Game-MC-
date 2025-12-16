#include "ErrorPopupPanel.h"

tgui::Panel::Ptr ErrorPopupPanel::Create(tgui::Gui& gui, const std::string& message, std::function<void()> onClose)
{
   
    auto errorPopupPanel = tgui::Panel::create({ "25%", "25%" });
    errorPopupPanel->setPosition({ "50%", "50%" });
    errorPopupPanel->setOrigin(0.5f, 0.5f);
    errorPopupPanel->getRenderer()->setBorderColor(tgui::Color::Black);
    errorPopupPanel->getRenderer()->setBorders(1);
    
    auto background = tgui::Picture::create("assets/images/background.png");
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
