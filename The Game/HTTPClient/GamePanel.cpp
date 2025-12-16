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
			if (messageSent.empty() || messageSent.size() > 500) return;

			std::thread([this, messageSent]()
				{
					if (!m_apiClient.SendChatMessage(messageSent))
					{
						m_chat->addLine("(ERROR) Failed to send message", tgui::Color::Red);
					}
				}).detach();

			chatEditBox->setText("");
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
				toggleChatButton->setText("<");
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
	if (m_chatClock.getElapsedTime().asMilliseconds() < 50) return;

	m_chatClock.restart();

	const auto& newMessages = m_apiClient.GetNewMessages(m_lastMessageId);
	if (!newMessages || newMessages->empty()) return;

	for (const auto& newMessage : newMessages.value())
	{
		m_chat->addLine(newMessage.username + ": " + newMessage.message);
		m_lastMessageId = std::max(newMessage.id, m_lastMessageId);
	}

}

void GamePanel::SetupPlayerSlots()
{
	auto playerUsernames = m_apiClient.GetLobbyPlayers(m_apiClient.GetLobbyId());
	const std::string currentPlayer = m_apiClient.GetUsername();

	auto it = std::find(playerUsernames.begin(), playerUsernames.end(), currentPlayer);
	if (it != playerUsernames.end())
	{
		std::rotate(playerUsernames.begin(), it, playerUsernames.end());
	}

	const auto slots = GetSlotsForPlayerCount(playerUsernames.size());
	if (slots.empty()) return;

	for (std::size_t index = 0; index < playerUsernames.size(); index++)
	{
		auto userPanel = tgui::Panel::create({ "14%", "12%" });
		userPanel->setPosition(
			tgui::String(std::to_string(slots[index].xPercent) + "%"),
			tgui::String(std::to_string(slots[index].yPercent) + "%")
		);
		userPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		userPanel->getRenderer()->setBorders(1);
		userPanel->getRenderer()->setBorderColor(tgui::Color::Black);
		userPanel->getRenderer()->setOpacity(0.95f);

		auto infoPanel = tgui::Panel::create({"70%", "100%"});
		infoPanel->setPosition({ "30%", "0%" });
		infoPanel->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40));

		auto userLabel = tgui::Label::create(Utils::TruncateText(
			tgui::String(playerUsernames[index]), kmaxChars));
		userLabel->setAutoSize(false);
		userLabel->setSize({ "90%", "40%" });
		userLabel->setPosition({ "50%", "50%" });
		userLabel->setOrigin(0.5f, 0.5f);
		userLabel->setTextSize(20);
		userLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		userLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		userLabel->getRenderer()->setTextColor(tgui::Color::White);

		if (index == 0)
		{
			userLabel->getRenderer()->setTextColor(tgui::Color::Yellow);
		}

		infoPanel->add(userLabel);

		auto avatarPanel = tgui::Panel::create({"32.5%", "100%"});
		avatarPanel->setPosition({ "0%", "0%" });
		avatarPanel->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40));

		auto userAvatar = tgui::Picture::create("assets/images/playerDefaultAvatar.jpg");
		userAvatar->setSize({ "100%", "100%" });
		userAvatar->setPosition({ "0%", "0%" });
		userAvatar->setIgnoreMouseEvents(true);
		
		avatarPanel->add(userAvatar);

		userPanel->add(avatarPanel);
		userPanel->add(infoPanel);
		m_root->add(userPanel);
	}
}

void GamePanel::OnGameStarted()
{
	SetupPlayerSlots();
}
