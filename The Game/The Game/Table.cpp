#include "Table.h"

constexpr static unsigned int HOURS_TO_SECONDS = 3600;

constexpr static uint8_t  TWO_PLAYERS = 2;
constexpr static uint8_t  CARDS_TWO_PLAYERS = 8;

constexpr static uint8_t THREE_PLAYERS = 3;
constexpr static uint8_t  CARDS_THREE_PLAYERS = 7;

constexpr static uint8_t  CARDS_DEFAULT_PLAYERS = 6;

constexpr static uint8_t MIN_CARDS_NORMAL = 2;
constexpr static uint8_t MIN_CARDS_EMPTY_DECK = 1;

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
	return m_Deck.empty();
}

void Table::Shuffle()
{
	std::vector<Card> copyCards;

	for (unsigned int i = 2; i <= 99; i++)
		m_Deck.push(Card(i));

	while (!m_Deck.empty())
	{
		copyCards.push_back(std::move(m_Deck.top()));
		m_Deck.pop();
	}

	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(copyCards.begin(), copyCards.end(), generator);
	
	for (const auto& card : copyCards)
		m_Deck.push(std::move(card));
}

void Table::SetupPlayers()
{
	m_Players.reserve(m_nrPlayers);
	switch (m_nrPlayers)
	{
	case TWO_PLAYERS:
		for (auto& player : m_Players)
		{
			player.SetupCards(m_Deck, CARDS_TWO_PLAYERS);
		}
		break;

	case THREE_PLAYERS:
		for (auto& player : m_Players)
		{
			player.SetupCards(m_Deck, CARDS_THREE_PLAYERS);
		}
		break;

	default:
		for (auto& player : m_Players)
		{
			player.SetupCards(m_Deck, CARDS_DEFAULT_PLAYERS);
		}
		break;

	};
}

bool Table::BackwardsTrick(const Deck& deck, const Card& card)
{
	const unsigned int cardValue = card.getId();
	const unsigned int topCardValue = deck.GetTopCard().getId();

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

bool Table::CheckCard(const Deck& deck, const Card& card)
{
	const unsigned int cardValue = card.getId();
	const unsigned int topCardValue = deck.GetTopCard().getId();

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
}

const std::stack<Card>& Table::GetMainDeck() const
{
	return m_Deck;
}

void Table::PlayGame()
{
	bool gameRunning = true;
	bool gameWon = false;
	uint8_t currentPlayerIndex = 0;
	std::cout << "\nThe game started!\n";
	std::cout << "Number of players: " << static_cast<int>(m_nrPlayers) << "\n";
	std::cout << "Cards in  deck: " << m_Deck.size() << "\n";
	while (gameRunning)
	{
		if (AllCardsPlaced())
		{
			std::cout << "Congratulations! All 98 cards placed!\n";
			gameWon = true;
			gameRunning = false;
			break;
		}
	}
	double hours = static_cast<double>(m_time / HOURS_TO_SECONDS);
	for (Player player : m_Players)
	{
		player.GetUserProfile().UpdateStats(hours, gameWon, player.GetCardsCount());
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
	uint8_t cardsPlaced = 0;
	bool continueTurn = true;

	while (continueTurn)
	{
		std::cout << "Your cards: ";
		player.PrintCards();

		if (cardsPlaced >= minCards)
		{
			std::cout << "You already placed " << static_cast<int>(cardsPlaced) << " cards.(minimum = " << static_cast<int>(minCards) << ")\n";
			std::cout << "Would you like to place another card? (1=Yes, 0=No): ";

			int choice;
			std::cin >> choice;
			if (choice == 0)
			{
				continueTurn = false;
				break;
			}
		}
		std::cout << "Choose card's ID you want to place: ";
		unsigned int cardId;
		std::cin >> cardId;

		if (!player.FindCard(cardId))
		{
			std::cout << "Invalid card!\n";
			continue;
		}

		std::cout << "Choose deck:\n";
		DisplayTableState();
		std::cout << "Choice: ";
		int deckChoice;
		std::cin >> deckChoice;

		Deck* chosenDeck = nullptr;
		switch (deckChoice)
		{
		case 1: chosenDeck = &m_AscendingDeckFirst; break;
		case 2: chosenDeck = &m_AscendingDeckSecond; break;
		case 3: chosenDeck = &m_DescendingDeckFirst; break;
		case 4: chosenDeck = &m_DescendingDeckSecond; break;
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
		uint8_t cardsToDraw = std::min(static_cast<uint8_t>(cardsPlaced), static_cast<uint8_t>(m_Deck.size()));
		std::cout << "You draw " << static_cast<int>(cardsToDraw) << " new cards.\n";
		player.DrawCards(m_Deck);
	}
	return cardsPlaced >= minCards;
}
