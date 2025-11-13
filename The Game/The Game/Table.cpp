#include "Table.h"

constexpr static unsigned int HOURS_TO_SECONDS = 3600;

constexpr static uint8_t  TWO_PLAYERS = 2;
constexpr static uint8_t  CARDS_TWO_PLAYERS = 8;

constexpr static uint8_t THREE_PLAYERS = 3;
constexpr static uint8_t  CARDS_THREE_PLAYERS = 7;

constexpr static uint8_t  CARDS_DEFAULT_PLAYERS = 6;



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
	while (gameRunning)
	{

	}
	double hours = static_cast<double>(m_time / HOURS_TO_SECONDS);
	for (Player player : m_Players)
	{
		player.GetUserProfile().UpdateStats(hours, gameWon, player.GetCardsCount());
	}
}
