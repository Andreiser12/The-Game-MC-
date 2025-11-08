#pragma once
#include <vector>
#include <stack>
#include <algorithm>
#include <random>
#include "Player.h"
#include "Cards.h"
#include "Deck.h"


class Table
{
public:
	Table(unsigned int nrPlayers, const std::stack<Cards>&deck);
	bool isDeckEmpty() const;
	void Shuffle();
	bool BackwardsTrick(const Deck& deck, const Cards& card);
	bool CheckCard(const Deck& deck, const Cards& card);
	Deck& getDeck(const Deck::DeckType &type, const Deck::DeckNumber &number);
private:
	unsigned int m_nrPlayers;
	std::vector<Player> m_Players;
	std::stack<Cards> m_Deck;
	Deck m_AscendingDeckFirst;
	Deck m_AscendingDeckSecond;
	Deck m_DescendingDeckFirst;
	Deck m_DescendingDeckSecond;
};

