#include "GamePanel.h"

GamePanel::GamePanel(tgui::Gui& gui, ApiClient& api) :
	m_apiClient{ api }
{
	m_root = tgui::Group::create({ "100%", "100%" });
	gui.add(m_root);

	auto gamePanel = tgui::Panel::create({ "100%", "100%" });
	m_root->add(gamePanel);

	auto gameBackground = tgui::Picture::create("assets/images/gameBackground.jpg");
	gameBackground->setSize({ "100%", "100%" });
	gameBackground->setPosition({ "0%", "0%" });
	gamePanel->add(gameBackground);
	gameBackground->moveToBack();

	auto canvas = tgui::CanvasSFML::create();
	canvas->setSize({ "70%", "50%" });
	canvas->setPosition({ "50%", "50%" });
	canvas->setOrigin(0.5f, 0.5f);
	gamePanel->add(canvas);

	canvas->clear(tgui::Color::Transparent);

	const sf::Vector2f canvasSize{
		static_cast<float>(canvas->getSize().x),
		static_cast<float>(canvas->getSize().y)
	};

	const float baseSize = std::min(canvasSize.x, canvasSize.y);
	const float tableRadius = baseSize * 0.35f;

	sf::CircleShape tableShape;
	tableShape.setRadius(tableRadius);
	tableShape.setPointCount(100);
	tableShape.setFillColor(sf::Color(30, 75, 55));
	tableShape.setOutlineThickness(baseSize * 0.01f);
	tableShape.setOutlineColor(sf::Color(0, 0, 0));
	tableShape.setScale({ 2.f, 1.2f });

	float tableWidth = tableShape.getRadius() * 2.f * tableShape.getScale().x;
	float tableHeight = tableShape.getRadius() * 2.f * tableShape.getScale().y;

	tableShape.setPosition(
		{ (canvasSize.x - tableWidth) / 2.f,
		(canvasSize.y - tableHeight) / 2.f }
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
	m_chat->getRenderer()->setOpacity(0.7f);
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
						m_chat->addLine("[Error]: Message has not been sent", tgui::Color::Red);
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
			if (messageSent.empty() || messageSent.size() > 500) return;

			std::thread([this, messageSent]()
				{
					if (!m_apiClient.SendChatMessage(messageSent))
					{
						m_chat->addLine("[Error]: Message has not been sent", tgui::Color::Red);
					}
				}).detach();

			chatEditBox->setText("");
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

	m_decks[0] = std::make_shared<DeckWidget>(DeckWidget::DeckType::Ascending, 0u);
	m_decks[1] = std::make_shared<DeckWidget>(DeckWidget::DeckType::Ascending, 1u);
	m_decks[2] = std::make_shared<DeckWidget>(DeckWidget::DeckType::Descending, 2u);
	m_decks[3] = std::make_shared<DeckWidget>(DeckWidget::DeckType::Descending, 3u);
	float xDeckPosition{ 42.5f };
	constexpr float yDeckPosition{ 46.25f };
	for (const auto& deck : m_decks)
	{
		auto deckPanel = deck->GetDeckPanel();
		deckPanel->setSize({ "2.25%", "6.5%" });
		deckPanel->setPosition({
			tgui::String(std::to_string(xDeckPosition) + "%"),
			tgui::String(std::to_string(yDeckPosition) + "%")
			});
		xDeckPosition += 3.75f;
		m_root->add(deckPanel);
	}
	m_pileWidget = std::make_shared<PileWidget>(PileWidget());
	auto pilePanel = m_pileWidget->GetPilePanel();
	pilePanel->setSize({ "2.25%", "6.5%" });
	pilePanel->setPosition({
		tgui::String(std::to_string(xDeckPosition) + "%"),
		tgui::String(std::to_string(yDeckPosition) + "%")
		});
	m_root->add(pilePanel);
}

void GamePanel::Update()
{
	RemovePlacedCards();
	auto newMessages = m_apiClient.SnapshotNewChat();
	if (!newMessages || newMessages->empty()) return;

	for (const auto& message : newMessages.value())
	{
		m_chat->addLine(message.username + ": " + message.message);
		m_lastMessageId = std::max(m_lastMessageId, message.id);
	}
}

void GamePanel::SetupPlayerSlots()
{
	auto playerUsernames = m_apiClient.SnapshotPlayers();
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

		auto infoPanel = tgui::Panel::create({ "70%", "100%" });
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
			CreateMyHand(slots[index]);
		}
		else
		{
			CreatePlayerHand(index, slots[index]);
		}

		infoPanel->add(userLabel);

		auto avatarPanel = tgui::Panel::create({ "32.5%", "100%" });
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

void GamePanel::HandleMouseEvent(const sf::Event& event)
{
	if (!event.is<sf::Event::MouseMoved>()) return;

	const auto& mouseMove = event.getIf<sf::Event::MouseMoved>();
	if (!mouseMove) return;

	tgui::Vector2f mousePos(
		static_cast<float>(mouseMove->position.x),
		static_cast<float>(mouseMove->position.y));

	for (auto& card : m_myCards)
	{
		card->UpdateDragPosition(mousePos);
		for (const auto& deck : m_decks)
		{
			card->CheckOnDeck(deck);
		}
	}
}

void GamePanel::ResetAllDrags()
{
	for (auto& card : m_myCards)
	{
		card->CancelDrag();
	}
}

void GamePanel::RemovePlacedCards()
{
	m_myCards.erase(
		std::remove_if(m_myCards.begin(), m_myCards.end(),
			[this](const std::shared_ptr<CardWidget> card)
			{
				if (card->IsPlacedOnDeck())
				{
					m_root->remove(card->GetWidget());
					return true;
				}
				return false;
			}), 
	m_myCards.end());
}

void GamePanel::CreatePlayerHand(size_t playerIndex, const PlayerSlot& slot)
{
	constexpr float cardWidth{ 2.25f };
	constexpr float cardHeight{ 6.5f };
	constexpr float cardSpacing{ 0.20f };
	auto playerCards = m_apiClient.GetPlayerCards();
	const size_t cardCount = playerCards.has_value() ? playerCards.value().size() : 3;

	const float totalWidth{ (cardWidth + cardSpacing) * cardCount - cardSpacing };
	float startX{ slot.xPercent + 7.0f - (totalWidth / 2.0f) };
	float startY{ slot.yPercent - cardHeight };

	for (size_t i = 0; i < cardCount; i++)
	{
		auto card = std::make_shared<CardWidget>(0u, false);
		card->SetSize(
			tgui::String(std::to_string(cardWidth) + "%"),
			tgui::String(std::to_string(cardHeight) + "%")
		);
		float cardX{ startX + i * (cardWidth + cardSpacing) };
		card->SetPosition(
			tgui::String(std::to_string(cardX) + "%"),
			tgui::String(std::to_string(startY) + "%")
		);
		m_root->add(card->GetWidget());
	}
}

void GamePanel::CreateMyHand(const PlayerSlot& slot)
{
	const auto& playerCards = m_apiClient.GetPlayerCards();
	if (!playerCards) return;

	constexpr float cardWidth{ 2.25f };
	constexpr float cardHeight{ 6.5f };
	constexpr float cardSpacing{ 0.20f };

	const size_t cardCount = playerCards.value().size();
	const float totalWidth{ (cardWidth + cardSpacing) * cardCount - cardSpacing };
	const float startX{ slot.xPercent + 7.0f - (totalWidth / 2.0f) };
	const float startY{ slot.yPercent - cardHeight };

	for (size_t i = 0; i < cardCount; i++)
	{
		auto card = std::make_shared<CardWidget>(playerCards.value()[i], true);
		card->SetSize(
			tgui::String(std::to_string(cardWidth) + "%"),
			tgui::String(std::to_string(cardHeight) + "%")
		);
		const float cardX = startX + i * (cardWidth + cardSpacing);
		card->SetPosition(
			tgui::String(std::to_string(cardX) + "%"),
			tgui::String(std::to_string(startY) + "%")
		);
		card->onCardPlaced = [this](uint8_t cardId, std::shared_ptr<DeckWidget> deck) 
		{
			return m_apiClient.PlayCard(cardId, deck->GetDeckId());
		};
		card->SetDraggable(true);
		m_root->add(card->GetWidget());
		m_myCards.emplace_back(card);
	}
}
