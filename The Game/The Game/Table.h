#pragma once
#include <vector>
#include <stack>
#include "Player.h"
#include "Cards.h"
class Table
{
public:
	Table(unsigned int nrPlayers, std::stack<Cards> deck);
	bool isDeckEmpty() const;
private:
	unsigned int m_nrPlayers;
	std::vector<Player> m_Players;
	std::stack<Cards> m_Deck;
	std::stack<Cards> m_AscendingDeck;
	std::stack<Cards> m_DescendingDeck;
};

