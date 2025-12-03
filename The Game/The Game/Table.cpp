module Table;

constexpr static unsigned HOURS_TO_SECONDS{ 3600u };

constexpr static uint8_t  TWO_PLAYERS{ 2u };
constexpr static uint8_t  CARDS_TWO_PLAYERS{ 8u };

constexpr static uint8_t THREE_PLAYERS{ 3u };
constexpr static uint8_t  CARDS_THREE_PLAYERS{ 7u };

constexpr static uint8_t  CARDS_DEFAULT_PLAYERS{ 6u };

constexpr static uint8_t MIN_CARDS_NORMAL{ 2u };
constexpr static uint8_t MIN_CARDS_EMPTY_DECK{ 1u };

Table::Table(uint8_t nrPlayers) :
	m_nrPlayers{ nrPlayers }
{
	m_AscendingDeckFirst = Deck(Deck::DeckType::Ascending, Deck::DeckNumber::First);
	m_AscendingDeckSecond = Deck(Deck::DeckType::Ascending, Deck::DeckNumber::Second);
	m_DescendingDeckFirst = Deck(Deck::DeckType::Descending, Deck::DeckNumber::First);
	m_DescendingDeckSecond = Deck(Deck::DeckType::Descending, Deck::DeckNumber::Second);
	m_time = 0u;
	Shuffle();
	SetupPlayers();

	std::sort(m_Players.begin(), m_Players.end());
}

bool Table::isDeckEmpty() const
{
	return m_mainDeck.empty();
}

void Table::Shuffle()
{
	std::vector<Card> copyCards;
	copyCards.reserve(98);

	for (uint16_t i = 2u; i <= 99u; ++i)
		copyCards.emplace_back(i);

	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(copyCards.begin(), copyCards.end(), generator);
	
	for (auto& card : copyCards)
		m_mainDeck.push(std::move(card));
}

void Table::SetupPlayers()
{
	DatabaseManager database{ "gameDB.db" };
	auto players = database.GetUsers();
	for (uint8_t i = 0; i < m_nrPlayers && i<players.size(); i++)
	{
		const auto& player = players[i];
		UserProfile profile(player.GetUsername());
		std::unordered_set<Card, CardsSetHashFunction, CardsEqual> emptyCards;
		m_Players.push_back(Player(player.GetID(), emptyCards, profile));
	}
	uint8_t cardsPerPlayer;
	switch (m_nrPlayers)
	{
	case TWO_PLAYERS:
		cardsPerPlayer = CARDS_TWO_PLAYERS;
		break;

	case THREE_PLAYERS:
		cardsPerPlayer = CARDS_THREE_PLAYERS;
		break;

	default:
		cardsPerPlayer = CARDS_DEFAULT_PLAYERS;
		break;
	}
	for (auto& player : m_Players)
	{
		player.SetupCards(m_mainDeck, cardsPerPlayer);
	}
}

bool Table::BackwardsTrick(const Deck& deck, const Card& card) const
{
	const unsigned cardValue = card.getId();
	const unsigned topCardValue = deck.GetTopCard().getId();

	switch (deck.GetDeckType())
	{
		case Deck::DeckType::Ascending:
			return (topCardValue>=10u && topCardValue - 10u == cardValue);
		case Deck::DeckType::Descending:
			return (topCardValue + 10u == cardValue);
		default:
			return false;
	}
}

bool Table::CheckCard(const Deck& deck, const Card& card) const
{
	const unsigned cardValue = card.getId();
	const unsigned topCardValue = deck.GetTopCard().getId();

	switch (deck.GetDeckType())
	{
	case Deck::DeckType::Ascending:
		return cardValue > topCardValue || BackwardsTrick(deck, card);
	case Deck::DeckType::Descending:
		return cardValue < topCardValue || BackwardsTrick(deck, card);
	default:
		return false;
	}
}

Deck& Table::GetDeck(const Deck::DeckType& type, const Deck::DeckNumber& number)
{
	if (type == Deck::DeckType::Ascending)
	{
		if (number == Deck::DeckNumber::First)
			return m_AscendingDeckFirst;
		else if (number == Deck::DeckNumber::Second)
			return m_AscendingDeckSecond;
	}
	else if (type == Deck::DeckType::Descending)
	{
		if (number == Deck::DeckNumber::First)
			return m_DescendingDeckFirst;
		else if (number == Deck::DeckNumber::Second)
			return m_DescendingDeckSecond;
	}
	throw std::out_of_range("Invalid deck type or number.");
}

const std::stack<Card>& Table::GetMainDeck() const
{
	return m_mainDeck;
}

void Table::PlayGame()
{
	bool gameRunning = true;
	bool gameWon = false;
	uint8_t currentPlayerIndex = 0u;
	std::cout << "\nThe game started!\n";
	std::cout << "Number of players: " << static_cast<uint8_t>(m_nrPlayers) << "\n";
	std::cout << "Cards in  deck: " << m_mainDeck.size() << "\n";
	auto startTime = std::chrono::high_resolution_clock::now();

	while (gameRunning)
	{
		if (AllCardsPlaced())
		{
			std::cout << "Congratulations! All 98 cards placed!\n";
			gameWon = true;
			gameRunning = false;
			break;
		}
		Player& currentPlayer = m_Players[currentPlayerIndex];

		std::cout << "Player " << currentPlayer.GetUserProfile().GetUsername() << "'s turn.\n";
		std::cout << "Cards: " << currentPlayer.GetCardsCount() << "\n";
		std::cout << "Cards on deck: " << m_mainDeck.size() << "\n";

		DisplayTableState();

		uint8_t minCards = isDeckEmpty() ? MIN_CARDS_EMPTY_DECK : MIN_CARDS_NORMAL;

		if (!CanPlayerPlayMinCards(currentPlayer, minCards))
		{
			std::cout << currentPlayer.GetUserProfile().GetUsername() << " can't place " <<
				static_cast<uint8_t>(minCards) << " cards! Game Over!\n";
			gameWon = false;
			gameRunning = false;
			break;
		}
		bool turnSucces = PlayerTurn(currentPlayer, minCards);

		if (!turnSucces)
		{
			std::cout << "Error while placing cards! Game Over!\n";
			gameWon = false;
			gameRunning = false;
			break;
		}
		currentPlayerIndex = (currentPlayerIndex + 1) % m_nrPlayers;
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration(endTime - startTime);
	m_time = static_cast<unsigned>(duration.count());

	double hours = static_cast<double>(m_time / HOURS_TO_SECONDS);
	std::cout << "Final game stats:\n";
	std::cout << "Result: " << (gameWon ? "Victory!" : "Defeat!") << "\n";
	std::cout << "Time played: " << hours << " hours\n";
	for (Player& player : m_Players)
	{
		size_t finalCards = player.GetCardsCount();
		player.GetUserProfile().UpdateStats(hours, gameWon, finalCards);
		std::cout << player.GetUserProfile().GetUsername() << " - Cards left: " << finalCards << "\n";
	}
}

void Table::DisplayTableState() const
{
	std::cout << "Deck state:\n";
	std::cout << "Ascending first: " << m_AscendingDeckFirst.GetTopCard().getId() << "\n";
	std::cout << "Ascending second: " << m_AscendingDeckSecond.GetTopCard().getId() << "\n";
	std::cout << "Descending first: " << m_DescendingDeckFirst.GetTopCard().getId() << "\n";
	std::cout << "Descending second: " << m_DescendingDeckSecond.GetTopCard().getId() << "\n";
}

bool Table::CanPlayerPlayMinCards(const Player& player, uint8_t minCards) 
{
	if (player.GetCardsCount() < minCards)
		return false;
	uint8_t validCardsCount = 0;
	for (const Card& card : player.GetCards())
	{
		if (CheckCard(m_AscendingDeckFirst, card) ||
			CheckCard(m_AscendingDeckSecond, card) ||
			CheckCard(m_DescendingDeckFirst, card) ||
			CheckCard(m_DescendingDeckSecond, card))
		{
			validCardsCount++;
			if (validCardsCount >= minCards)
				return true;
		}
	}
	return false;
}

void Table::PlaceCardOnDeck(Deck& deck, const Card& card)
{
	deck.PushCard(card);
	std::cout << " [Card " << card.getId() << " placed on deck]\n";
}

bool Table::AllCardsPlaced() const
{
	if (!isDeckEmpty())
		return false;

	for (const Player& player : m_Players) {
		if (player.GetCardsCount() > 0)
			return false;
	}
	return true;
}

bool Table::PlayerTurn(Player& player, uint8_t minCards)
{
	uint8_t cardsPlaced = 0u;
	bool continueTurn = true;

	while (continueTurn)
	{
		std::cout << "Your cards: ";
		player.PrintCards();

		if (cardsPlaced >= minCards)
		{
			std::cout << "You already placed " << static_cast<int>(cardsPlaced) << " cards.(minimum = " << static_cast<int>(minCards) << ")\n";
			std::cout << "Would you like to place another card? (1=Yes, 0=No): ";

			unsigned choice;
			std::cin >> choice;
			if (choice == 0u)
			{
				continueTurn = false;
				break;
			}
		}
		std::cout << "Choose card's ID you want to place: ";
		uint16_t cardId;
		std::cin >> cardId;

		if (!player.FindCard(cardId))
		{
			std::cout << "Invalid card!\n";
			continue;
		}

		std::cout << "Choose deck:\n";
		std::cout << "1 - Ascending First\n";
		std::cout << "2 - Ascending Second\n";
		std::cout << "3 - Descending First\n";
		std::cout << "4 - Descending Second\n";
		DisplayTableState();
		std::cout << "Choice: ";
		unsigned deckChoice;
		std::cin >> deckChoice;

		Deck* chosenDeck = nullptr;
		switch (deckChoice)
		{
		case 1u:
			chosenDeck = &m_AscendingDeckFirst;
			break;
		case 2u:
			chosenDeck = &m_AscendingDeckSecond;
			break;
		case 3u:
			chosenDeck = &m_DescendingDeckFirst;
			break;
		case 4u:
			chosenDeck = &m_DescendingDeckSecond;
			break;
		default:
			std::cout << "Invalid choice\n";
			continue;
		}

		Card cardToPlace(cardId);
		if (!CheckCard(*chosenDeck, cardToPlace)) {
			std::cout << "The card can't be placed on this deck!\n";
			continue;
		}

		PlaceCardOnDeck(*chosenDeck, cardToPlace);
		player.RemoveCard(cardId);
		cardsPlaced++;

		std::cout << "Card " << cardId << " was placed succesfully!\n";

		if (player.GetCardsCount() == 0)
		{
			std::cout << "You ran out of cards!\n";
			continueTurn = false;
			break;
		}
	}
	if (!isDeckEmpty() && cardsPlaced > 0)
	{
		uint8_t cardsToDraw = std::min(static_cast<uint8_t>(cardsPlaced), static_cast<uint8_t>(m_mainDeck.size()));
		std::cout << "You draw " << static_cast<uint8_t>(cardsToDraw) << " new cards.\n";
		player.DrawCards(m_mainDeck);
	}
	return cardsPlaced >= minCards;
}
