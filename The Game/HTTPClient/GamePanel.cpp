#include "GamePanel.h"

GamePanel::GamePanel(tgui::Gui& gui, ApiClient& api) :
	m_apiClient{ api }
{
	m_root = tgui::Group::create({ "100%", "100%" });
	gui.add(m_root);

	auto gamePanel = tgui::Panel::create({ "100%", "100%" });
	m_root->add(gamePanel);

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

	m_chat = tgui::ChatBox::create();
	m_chat->setSize({ "27.5%", "25%" });
	m_chat->setPosition({ "0%", "0%" });
	m_chat->setTextSize(25);
	m_chat->setInheritedOpacity(0.7f);
	chatGroup->add(m_chat);

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
	chatEditBox->onReturnKeyPress([this, chatEditBox]()
		{
			const std::string messageSent = chatEditBox->getText().toStdString();
			if (messageSent.size())
			{
				m_apiClient.SendChatMessage(messageSent);
				chatEditBox->setText("");
			}
		});
	chatGroup->add(chatEditBox);

	auto sendButton = tgui::Button::create("Send");
	sendButton->setSize({ "5%", "5%" });
	sendButton->setPosition({ "22.5%", "25%" });
	sendButton->setTextSize(25);
	sendButton->onPress([this, chatEditBox]()
		{
			const std::string messageSent = chatEditBox->getText().toStdString();
			if (messageSent.size())
			{
				m_apiClient.SendChatMessage(messageSent);
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
}

void GamePanel::Update()
{
	if (m_chatClock.getElapsedTime().asMilliseconds() < 200) return;

	m_chatClock.restart();
	const auto newMessages = m_apiClient.GetNewMessages(m_lastMessageId);
	if (!newMessages) return;

	for (const auto& newMessage : *newMessages)
	{
		m_chat->addLine(newMessage.username + ": " + newMessage.message);
		m_lastMessageId = newMessage.id;
	}
}
