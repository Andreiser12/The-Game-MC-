#include "GamePanel.h"

GamePanel::GamePanel(tgui::Gui& gui, ApiClient& api,
	std::function<void()> onExit, std::function<void()> onLocalPlayerLeft) :
	m_gui{ gui }, m_apiClient{ api },
	m_onExitToMenu{ std::move(onExit) }, m_onLocalPlayerLeft{ std::move(onLocalPlayerLeft) }
{
	if (m_gameMusic.openFromFile("assets/sounds/12_Silent_Piano_with_Creeping_Tension.ogg"))
	{
		m_gameMusic.setLooping(true);
		m_gameMusic.setVolume(50.0f);
		m_gameMusic.play();
	}

	m_root = tgui::Group::create({ "100%", "100%" });
	m_gui.add(m_root);

	auto gamePanel = tgui::Panel::create({ "100%", "100%" });
	m_root->add(gamePanel);

	auto gameBackground = tgui::Picture::create("assets/images/gameBackground.jpg");
	gameBackground->setSize("100%", "100%");
	gameBackground->setPosition("0%", "0%");
	gamePanel->add(gameBackground);
	gameBackground->moveToBack();

	auto canvas = tgui::CanvasSFML::create();
	canvas->setSize("70%", "50%");
	canvas->setPosition("50%", "50%");
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
	chatGroup->setPosition("0%", "0%");
	gamePanel->add(chatGroup);

	m_chat = tgui::ChatBox::create();
	m_chat->setSize("27.5%", "25%");
	m_chat->setPosition("0%", "0%");
	m_chat->setTextSize(25);
	m_chat->getRenderer()->setOpacity(0.7f);
	chatGroup->add(m_chat);

	auto chatEditBox = tgui::EditBox::create();
	chatEditBox->setSize("22.5%", "5%");
	chatEditBox->setPosition("0", "25%");
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
	sendButton->setSize("5%", "5%");
	sendButton->setPosition("22.5%", "25%");
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
	toggleChatButton->setSize("3%", "3%");
	toggleChatButton->setPosition("24.5%", "0%");
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
		deckPanel->setSize("2.25%", "6.5%");
		deckPanel->setPosition({
			tgui::String(std::to_string(xDeckPosition) + "%"),
			tgui::String(std::to_string(yDeckPosition) + "%")
			});
		xDeckPosition += 3.75f;
		m_root->add(deckPanel);
	}
	m_pileWidget = std::make_shared<PileWidget>(PileWidget());
	auto pilePanel = m_pileWidget->GetPilePanel();
	pilePanel->setSize("2.25%", "6.5%");
	pilePanel->setPosition({
		tgui::String(std::to_string(xDeckPosition) + "%"),
		tgui::String(std::to_string(yDeckPosition) + "%")
		});
	m_root->add(pilePanel);

	m_endTurnButton = tgui::Button::create("End Turn");
	m_endTurnButton->setSize("14%", "9%");
	m_endTurnButton->setPosition("85%", "90%");
	m_endTurnButton->getRenderer()->setBorders(2);
	m_endTurnButton->getRenderer()->setBorderColor(tgui::Color(100, 20, 30));
	m_endTurnButton->onSizeChange([this](tgui::Vector2f size)
		{
			m_endTurnButton->getRenderer()->setRoundedBorderRadius(size.y * 0.35f);
		});
	m_endTurnButton->getRenderer()->setTextSize(22);
	m_endTurnButton->getRenderer()->setRoundedBorderRadius(10);
	m_endTurnButton->setEnabled(false);
	m_endTurnButton->getRenderer()->setBackgroundColor(tgui::Color(150, 30, 45));
	m_endTurnButton->getRenderer()->setBackgroundColorHover(tgui::Color(175, 40, 55));
	m_endTurnButton->getRenderer()->setBackgroundColorDown(tgui::Color(120, 25, 35));
	m_endTurnButton->getRenderer()->setBackgroundColorDisabled(tgui::Color(90, 70, 75));
	m_endTurnButton->onPress([this]()
		{
			auto drawnCards = m_apiClient.EndTurn();
			if (!drawnCards) return;

			RebuildMyHand();
			m_endTurnButton->setEnabled(false);

		});
	m_root->add(m_endTurnButton);

	m_leaveMatchButton = tgui::Button::create("Leave Match");
	m_leaveMatchButton->setSize("14%", "9%");
	m_leaveMatchButton->setPosition("1%", "90%");
	m_leaveMatchButton->getRenderer()->setBorders(2);
	m_leaveMatchButton->onSizeChange([this](tgui::Vector2f size)
		{
			m_leaveMatchButton->getRenderer()->setRoundedBorderRadius(size.y * 0.35f);
		});
	m_leaveMatchButton->getRenderer()->setTextSize(22);
	m_leaveMatchButton->getRenderer()->setRoundedBorderRadius(10);
	m_leaveMatchButton->getRenderer()->setBackgroundColor(tgui::Color(75, 75, 80));
	m_leaveMatchButton->getRenderer()->setTextColor(tgui::Color::White);
	m_leaveMatchButton->getRenderer()->setBorderColor(tgui::Color(95, 95, 100));
	m_leaveMatchButton->getRenderer()->setBackgroundColorHover(tgui::Color(85, 40, 45));
	m_leaveMatchButton->getRenderer()->setBorderColorHover(tgui::Color(140, 60, 65));
	m_leaveMatchButton->getRenderer()->setBackgroundColorDown(tgui::Color(60, 60, 65));
	m_leaveMatchButton->onPress([this]()
		{
			auto result = m_apiClient.LeaveMatch();
			if (!result) return;

			if (result == LeaveMatchResult::Success)
			{
				if (m_onLocalPlayerLeft) m_onLocalPlayerLeft();
			}
		});
	m_root->add(m_leaveMatchButton);

	m_timerLabel = tgui::Label::create();
	m_timerLabel->setPosition("85%", "5%");
	m_timerLabel->setSize("12%", "6%");
	m_timerLabel->setText("01:00");
	m_timerLabel->setTextSize(24);
	m_timerLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	m_timerLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);

	m_timerLabel->getRenderer()->setTextColor(tgui::Color::White);
	m_timerLabel->getRenderer()->setBorders(2);
	m_timerLabel->getRenderer()->setBorderColor(tgui::Color(50, 100, 200));
	m_timerLabel->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 150));

	m_root->add(m_timerLabel);
}

void GamePanel::UpdateGame()
{
	if (m_gameFinished) return;

	float deltaTime = m_deltaTimeClock.restart().asSeconds();

	if (m_isMyTurn)
	{
		m_timeLeft -= deltaTime;

		if (m_timeLeft <= 0.0f)
		{
			m_timeLeft = 0.0f;
			OnTurnTimeout();
		}

		uint8_t seconds = static_cast<uint8_t>(m_timeLeft);
		std::string secStr = std::to_string(seconds);
		if (seconds < 10)
		{
			secStr = "0" + secStr;
		}

		m_timerLabel->setText("00:" + secStr);

		if (seconds <= 10)
		{
			m_timerLabel->getRenderer()->setTextColor(tgui::Color::Red);
		}
		else
		{
			m_timerLabel->getRenderer()->setTextColor(tgui::Color::White);
		}
	}
	else
	{
		m_timerLabel->setText("WAIT");
		m_timerLabel->getRenderer()->setTextColor(tgui::Color(200, 200, 200));
	}

	RemovePlacedCards();
	UpdateDecks();
	UpdateChat();
	UpdateEndTurnButton();
	UpdateTurnState();
	UpdateTableState();
	UpdatePlayerStates();

	const auto gameResultOpt = m_apiClient.SnapshotGameResult();
	if (gameResultOpt && gameResultOpt.value() != GameResult::Ongoing)
	{
		m_gameFinished = true;
		HandleGameResult(gameResultOpt.value());
	}
}

void GamePanel::UpdateDecks()
{
	auto decksState = m_apiClient.SnapshotDecks();
	if (!decksState) return;

	for (const auto& deckState : decksState.value())
	{
		if (deckState.deckId < m_decks.size())
		{
			m_decks[deckState.deckId]->SetLastId(deckState.topCardId);
		}
	}
}

void GamePanel::UpdateEndTurnButton()
{
	auto endTurnState = m_apiClient.SnapshotEndTurnState();
	if (!endTurnState) return;
	m_endTurnButton->setEnabled(endTurnState.value().canEndTurn);
}

void GamePanel::UpdateChat()
{
	auto newMessages = m_apiClient.SnapshotNewChat();
	if (!newMessages) return;

	for (const auto& message : newMessages.value())
	{
		m_chat->addLine(message.username + ": " + message.message);
		m_lastMessageId = std::max(m_lastMessageId, message.id);
	}
}

void GamePanel::UpdateTableState()
{
	auto tableState = m_apiClient.SnapshotGameTableState();
	if (!tableState) return;
	m_pileWidget->UpdatePile(tableState->drawPileCount);
}

void GamePanel::UpdateTurnState()
{
	auto turnState = m_apiClient.SnapshotTurnState();
	if (!turnState) return;

	bool serverSaysItIsMyTurn = turnState.value().isMyTurn;

	if (serverSaysItIsMyTurn && !m_isMyTurn)
	{
		m_timeLeft = 60.0f;
		m_timerLabel->getRenderer()->setTextColor(tgui::Color::White);
	}

	m_isMyTurn = turnState.value().isMyTurn;

	for (auto& card : m_myCards)
	{
		card->SetDraggable(m_isMyTurn);
	}
}

void GamePanel::UpdatePlayerStates()
{
	const auto& playerStates = m_apiClient.SnapshotPlayerStates();
	if (!playerStates) return;

	for (const auto& state : playerStates.value())
	{
		if (!state.isInMatch) MarkPlayerAsLeft(state.username);
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

	for (size_t index = 0; index < playerUsernames.size(); index++)
	{
		PlayerSlotWidget slotWidget;
		slotWidget.username = playerUsernames[index];

		slotWidget.panel = tgui::Panel::create({ "14%", "12%" });
		slotWidget.panel->setPosition(
			tgui::String(std::to_string(slots[index].xPercent) + "%"),
			tgui::String(std::to_string(slots[index].yPercent) + "%")
		);
		slotWidget.panel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
		slotWidget.panel->getRenderer()->setBorders(1);
		slotWidget.panel->getRenderer()->setBorderColor(tgui::Color::Black);
		slotWidget.panel->getRenderer()->setOpacity(0.95f);

		auto infoPanel = tgui::Panel::create({ "70%", "100%" });
		infoPanel->setPosition("30%", "0%");
		infoPanel->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40));

		slotWidget.nameLabel = tgui::Label::create(Utils::TruncateText(
			tgui::String(playerUsernames[index]), kmaxChars));
		slotWidget.nameLabel->setAutoSize(false);
		slotWidget.nameLabel->setSize("90%", "40%");
		slotWidget.nameLabel->setPosition("50%", "50%");
		slotWidget.nameLabel->setOrigin(0.5f, 0.5f);
		slotWidget.nameLabel->getRenderer()->setTextSize(20);
		slotWidget.nameLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		slotWidget.nameLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
		slotWidget.nameLabel->getRenderer()->setTextColor(tgui::Color::White);

		if (index == 0)
		{
			m_mySlot = slots[index];
			CreateMyHand();
		}
		else
		{
			CreatePlayerHand(index, slots[index], slotWidget.cards);
		}

		infoPanel->add(slotWidget.nameLabel);

		auto avatarPanel = tgui::Panel::create({ "32.5%", "100%" });
		avatarPanel->setPosition("0%", "0%");
		avatarPanel->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40));

		auto userAvatar = tgui::Picture::create("assets/images/playerDefaultAvatar.jpg");
		userAvatar->setSize("100%", "100%");
		userAvatar->setPosition("0%", "0%");
		userAvatar->setIgnoreMouseEvents(true);

		avatarPanel->add(userAvatar);
		slotWidget.panel->add(avatarPanel);
		slotWidget.panel->add(infoPanel);
		m_root->add(slotWidget.panel);
		m_playerSlots[slotWidget.username] = slotWidget;
	}
}

void GamePanel::OnGameStarted()
{
	SetupPlayerSlots();
}

void GamePanel::HandleMouseEvent(const sf::Event& event)
{
	if (!event.is<sf::Event::MouseMoved>() || !m_isMyTurn) return;

	const auto& mouseMove{ event.getIf<sf::Event::MouseMoved>() };
	if (!mouseMove) return;

	tgui::Vector2f mousePos
	{ static_cast<float>(mouseMove->position.x),
		static_cast<float>(mouseMove->position.y) };

	bool anyCardMoving{ false };
	uint8_t movingCardId{ 0u };
	for (auto& card : m_myCards)
	{
		auto oldPos{ card->GetWidget()->getPosition() };
		card->UpdateDragPosition(mousePos);
		auto newPos{ card->GetWidget()->getPosition() };
		if (oldPos != newPos)
		{
			anyCardMoving = true;
			movingCardId = card->GetId();
		}

		for (const auto& deck : m_decks)
		{
			card->CheckOnDeck(deck);
		}
	}

	if (anyCardMoving)
	{
		auto validDecks = m_apiClient.SnapshotValidDecks(movingCardId);
		if (!validDecks) return;

		for (auto& deck : m_decks)
		{
			bool isValid = validDecks.value()[deck->GetDeckId()];
			bool isBackwards = false;

			uint8_t cardVal = static_cast<uint8_t>(movingCardId);
			uint8_t deckVal = static_cast<uint8_t>(deck->GetLastId());

			if (deck->GetDeckType() == DeckWidget::DeckType::Ascending)
			{
				if (cardVal == deckVal - 10) isBackwards = true;
			}
			else
			{
				if (cardVal == deckVal + 10) isBackwards = true;
			}

			if (isBackwards)
			{
				isValid = true;
			}

			deck->SetHighlight(isValid, isBackwards);
		}
	}
	else
	{
		for (const auto& deck : m_decks)
		{
			deck->ClearHighlight();
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

GamePanel::~GamePanel()
{
	m_gameMusic.stop();
	if (m_root) m_gui.remove(m_root);
}

void GamePanel::OnTurnTimeout()
{
	auto drawnCards = m_apiClient.EndTurn();
	m_isMyTurn = false;
	if (drawnCards)
	{
		RebuildMyHand();
		m_endTurnButton->setEnabled(false);
	}
}

void GamePanel::CreatePlayerHand(size_t playerIndex, const PlayerSlot& slot,
	std::vector<std::shared_ptr<CardWidget>>& outCards)
{
	const auto& playerCards = m_apiClient.GetPlayerCards();
	if (!playerCards) return;

	constexpr float cardWidth{ 2.25f };
	constexpr float cardHeight{ 6.5f };
	constexpr float cardSpacing{ 0.20f };
	const size_t cardCount{ playerCards.value().size() };
	const float totalWidth{ (cardWidth + cardSpacing) * cardCount - cardSpacing };
	const float startX{ slot.xPercent + 7.0f - (totalWidth / 2.0f) };
	const float startY{ slot.yPercent - cardHeight };

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
		outCards.emplace_back(card);
	}
}

void GamePanel::CreateMyHand()
{
	const auto& playerCards = m_apiClient.GetPlayerCards();
	if (!playerCards) return;

	constexpr float cardWidth{ 2.25f };
	constexpr float cardHeight{ 6.5f };
	constexpr float cardSpacing{ 0.20f };
	const size_t cardCount = playerCards.value().size();
	const float totalWidth{ (cardWidth + cardSpacing) * cardCount - cardSpacing };
	const float startX{ m_mySlot->xPercent + 7.0f - (totalWidth / 2.0f) };
	const float startY{ m_mySlot->yPercent - cardHeight };

	for (size_t i = 0; i < cardCount; i++)
	{
		auto card = std::make_shared<CardWidget>(playerCards.value()[i], true);
		card->SetSize(
			tgui::String(std::to_string(cardWidth) + "%"),
			tgui::String(std::to_string(cardHeight) + "%")
		);
		const float cardX{ startX + i * (cardWidth + cardSpacing) };
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

void GamePanel::ClearMyHand()
{
	for (auto& card : m_myCards)
	{
		m_root->remove(card->GetWidget());
	}
	m_myCards.clear();
}

void GamePanel::RebuildMyHand()
{
	ClearMyHand();
	CreateMyHand();
}

void GamePanel::HandleGameResult(GameResult result)
{
	if (result == GameResult::Ongoing) return;

	m_gameMusic.stop();

	for (auto& card : m_myCards)
	{
		card->SetDraggable(false);
	}

	m_endTurnButton->setEnabled(false);
	ShowGameResultOverlay(result);
}

void GamePanel::ShowGameResultOverlay(GameResult result)
{
	auto overlay = tgui::Panel::create({ "100%", "100%" });
	overlay->getRenderer()->setBackgroundColor(
		tgui::Color(0, 0, 0, 180)
	);

	auto dialog = tgui::Panel::create({ "40%", "30%" });
	dialog->setPosition("50%", "50%");
	dialog->setOrigin(0.5f, 0.5f);
	dialog->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40));
	dialog->getRenderer()->setBorders(2);
	dialog->getRenderer()->setBorderColor(tgui::Color::White);

	auto gameResultLabel = tgui::Label::create(
		result == GameResult::Won ? "YOU WON!" : "YOU LOST!"
	);
	gameResultLabel->getRenderer()->setTextSize(36);
	gameResultLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	gameResultLabel->setPosition("50%", "30%");
	gameResultLabel->setOrigin(0.5f, 0.5f);

	auto backToMenuButton = tgui::Button::create("Back to Menu");
	backToMenuButton->setSize("40%", "20%");
	backToMenuButton->setPosition("50%", "70%");
	backToMenuButton->setOrigin(0.5f, 0.5f);
	backToMenuButton->getRenderer()->setTextSize(20);
	backToMenuButton->onPress([this]()
		{
			if (m_onExitToMenu) m_onExitToMenu();
		});

	dialog->add(gameResultLabel);
	dialog->add(backToMenuButton);
	overlay->add(dialog);
	m_root->add(overlay);
}

void GamePanel::MarkPlayerAsLeft(const std::string& username)
{
	auto it = m_playerSlots.find(username);
	if (it == m_playerSlots.end()) return;

	auto& slot = it->second;
	if (!slot.isActive) return;

	slot.isActive = false;
	for (auto& card : slot.cards)
	{
		m_root->remove(card->GetWidget());
	}
	slot.cards.clear();

	slot.panel->getRenderer()->setBackgroundColor(tgui::Color(60, 60, 60, 100));
	slot.panel->getRenderer()->setBorderColor(tgui::Color(80, 80, 80));
	slot.nameLabel->setText(slot.nameLabel->getText() + " (Left)");
	slot.nameLabel->getRenderer()->setTextColor(tgui::Color(150, 150, 150));

	auto overlay = tgui::Panel::create({ "100%", "100%" });
	overlay->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40, 120));
	overlay->setIgnoreMouseEvents(true);
	slot.panel->add(overlay);
}
