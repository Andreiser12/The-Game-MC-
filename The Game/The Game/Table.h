#pragma once
#include <vector>
#include <stack>
#include "Player.h"
#include "Cards.h"
#include "Deck.h"

class Table
{
public:
	Table(unsigned int nrPlayers, const std::stack<Cards>&deck);
	bool isDeckEmpty() const;
private:
	unsigned int m_nrPlayers;
	std::vector<Player> m_Players;
	std::stack<Cards> m_Deck;
	Deck m_AscendingDeckFirst;
	Deck m_AscendingDeckSecond;
	Deck m_DescendingDeckFirst;
	Deck m_DescendingDeckSecond;
};

