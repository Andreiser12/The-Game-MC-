#include "Table.h"

Table::Table(unsigned int nrPlayers, const std::stack<Cards>& deck) : m_nrPlayers{ nrPlayers }, m_Deck(deck) {}

bool Table::isDeckEmpty() const
{
	return m_Deck.empty();
}
