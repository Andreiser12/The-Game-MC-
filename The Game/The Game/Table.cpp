#include "Table.h"


Table::Table(unsigned int nrPlayers, const std::stack<Cards>& deck) :
	m_nrPlayers{ nrPlayers }, m_Deck{ deck }
{
	m_AscendingDeckFirst = Deck(Deck::DeckType::Ascending, Deck::DeckNumber::First);
	m_AscendingDeckSecond = Deck(Deck::DeckType::Ascending, Deck::DeckNumber::Second);
	m_DescendingDeckFirst = Deck(Deck::DeckType::Descending, Deck::DeckNumber::First);
	m_DescendingDeckSecond = Deck(Deck::DeckType::Descending, Deck::DeckNumber::Second);
}

bool Table::isDeckEmpty() const
{
	return m_Deck.empty();
}

void Table::Shuffle()
{
	std::vector<Cards> copyCards;

	while (!m_Deck.empty())
	{
		copyCards.push_back(std::move(m_Deck.top()));
		m_Deck.pop();
	}

	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(copyCards.begin(), copyCards.end(), generator);
	
	for (Cards& card : copyCards)
		m_Deck.push(std::move(card));
}

bool Table::BackwardsTrick(const Deck& deck, const Cards& card)
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

bool Table::CheckCard(const Deck& deck, const Cards& card)
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

Deck& Table::getDeck(const Deck::DeckType& type, const Deck::DeckNumber& number)
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
