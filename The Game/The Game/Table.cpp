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
