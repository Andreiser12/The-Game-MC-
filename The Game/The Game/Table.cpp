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

std::stack<Cards> Table::getDeck() const
{
	return m_Deck;
}
