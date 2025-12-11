#include "GamePanel.h"

GamePanel::GamePanel(tgui::Gui& gui, ApiClient& api) :
    m_apiClient{api}
{
    auto gamePanel = Create(gui, api);
    gui.add(gamePanel);
}

tgui::Group::Ptr GamePanel::Create(tgui::Gui& gui, ApiClient& apiClient)
{
    auto group = tgui::Group::create({ "100%", "100%" });
    gui.add(group);
    auto gamePanel = tgui::Panel::create({ "100%", "100%" });
    group->add(gamePanel);

    auto canvas = tgui::CanvasSFML::create();
    canvas->setSize({ "70%", "50%" });
    canvas->setPosition({ "50%", "50%" });
    canvas->setOrigin(0.5f, 0.5f);
    gamePanel->add(canvas);

    canvas->clear(tgui::Color::Transparent);

    sf::CircleShape tableShape;
    tableShape.setRadius(200.f);
    tableShape.setPointCount(100);
    tableShape.setFillColor(sf::Color(20, 100, 20));
    tableShape.setOutlineThickness(10.f);
    tableShape.setOutlineColor(sf::Color(60, 30, 10));
    tableShape.setScale({ 2.f, 1.2f });

    float tableWidth = tableShape.getRadius() * 2.f * tableShape.getScale().x;
    float tableHeight = tableShape.getRadius() * 2.f * tableShape.getScale().y;

    tableShape.setPosition(
        { (canvas->getSize().x - tableWidth) / 2.f,
        (canvas->getSize().y - tableHeight) / 2.f }
    );

    canvas->draw(tableShape);
    canvas->display();

    auto chatGroup = tgui::Group::create({ "100%", "100%" });
    chatGroup->setPosition({ "0%", "0%" });
    gamePanel->add(chatGroup);

    auto gameChat = tgui::ChatBox::create();
    gameChat->setSize({ "27.5%", "25%" });
    gameChat->setPosition({ "0%", "0%" });
    gameChat->setTextSize(25);
    gameChat->setInheritedOpacity(0.7f);
    chatGroup->add(gameChat);

    auto chatEditBox = tgui::EditBox::create();
    chatEditBox->setSize({ "22.5%", "5%" });
    chatEditBox->setPosition({ "0", "25%" });
    chatEditBox->setTextSize(25);
    chatEditBox->getRenderer()->setOpacity(0.5f);
    chatEditBox->setDefaultText("Message...");
    chatEditBox->onFocus([chatEditBox]()
        {
            chatEditBox->setDefaultText("");
        });
    chatEditBox->onUnfocus([chatEditBox]()
        {
            chatEditBox->setDefaultText("Message...");
        });
    chatEditBox->onReturnKeyPress([chatEditBox, gameChat, &apiClient]()
        {
            const std::string messageSent = chatEditBox->getText().toStdString();
            if (messageSent.size())
            {
                if (apiClient.SendChatMessage(messageSent))
                {

                    //gameChat->addLine(messageSent);
                    chatEditBox->setText("");
                }
            }
        });
    chatGroup->add(chatEditBox);

    auto sendButton = tgui::Button::create("Send");
    sendButton->setSize({ "5%", "5%" });
    sendButton->setPosition({"22.5%", "25%"});
    sendButton->setTextSize(25);
    sendButton->onPress([chatEditBox, gameChat, &apiClient]()
        {
            const std::string messageSent = chatEditBox->getText().toStdString();
            if (apiClient.SendChatMessage(messageSent))
            {
                //gameChat->addLine(messageSent);
                chatEditBox->setText("");
            }
        });
    chatGroup->add(sendButton);

    auto toggleChatButton = tgui::Button::create("^");
    toggleChatButton->setSize({ "3%", "3%" });
    toggleChatButton->setPosition({ "24.5%", "0%" });
    toggleChatButton->setTextSize(25);
    toggleChatButton->onPress([toggleChatButton, chatGroup]()
        {
            if (toggleChatButton->getText().toStdString() == "^")
            {
                toggleChatButton->setText(">");
                chatGroup->setVisible(false);
            }
            else
            {
                toggleChatButton->setText("^");
                chatGroup->setVisible(true);
            }
        });
    gamePanel->add(toggleChatButton);

    return group;
}



void GamePanel::Update()
{
    /*if (m_chatClock.getElapsedTime().asMilliseconds() > 200)
    {
        auto newMessages = 
    }*/
}
