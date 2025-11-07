#pragma once
#include <vector>
#include <stack>
#include "Player.h"
#include "Cards.h"
class Table
{
public:
	Table(unsigned int nrPlayers, const std::stack<Cards>&deck);
	bool isDeckEmpty() const;
private:
	unsigned int m_nrPlayers;
	std::vector<Player> m_Players;
	std::stack<Cards> m_Deck;
	std::stack<Cards> m_AscendingDeckFirst;
	std::stack<Cards> m_AscendingDeckSecond;
	std::stack<Cards> m_DescendingDeckFirst;
	std::stack<Cards> m_DescendingDeckSecond;
};

