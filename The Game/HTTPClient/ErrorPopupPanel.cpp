#include "ErrorPopupPanel.h"

tgui::Panel::Ptr ErrorPopupPanel::Create(tgui::Gui& gui, const std::string& lobbyEntered)
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
    okButton->onPress([errorPopupPanel]()
        {
            errorPopupPanel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));

        });
    errorPopupPanel->add(okButton);

    auto errorLabel = tgui::Label::create();
    errorLabel->setSize({ "60%", "40%" });
    errorLabel->setPosition({ "50%", "45%" });
    errorLabel->setTextSize(24);
    errorLabel->setOrigin(0.5f, 0.5f);
    errorLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    errorLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    if (lobbyEntered.empty()) errorLabel->setText("Lobby ID can't be empty");
    else if (lobbyEntered.size() != 6u) errorLabel->setText("Lobby must be 6 characters long");
    else errorLabel->setText("Invalid Lobby ID");
    errorPopupPanel->add(errorLabel);

    return errorPopupPanel;
}
