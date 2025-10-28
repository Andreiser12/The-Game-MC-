#include "Table.h"

Table::Table(unsigned int nrPlayers, std::stack<Cards> deck) : m_nrPlayers{ nrPlayers }, m_Deck(deck) {}
