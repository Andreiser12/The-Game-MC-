#pragma once
#include <vector>
#include <stack>
#include <algorithm>
#include <random>

#include "Player.h"
#include "Card.h"
#include "Deck.h"

class Table
{
public:
	Table(uint8_t nrPlayers, const std::stack<Card>&deck);

	bool isDeckEmpty() const;
	void Shuffle();
	bool BackwardsTrick(const Deck& deck, const Card& card);
	bool CheckCard(const Deck& deck, const Card& card);
	void PlayGame();


	Deck& GetDeck(const Deck::DeckType &type, const Deck::DeckNumber &number);
	const std::stack<Card>& GetMainDeck() const;

private:
	uint8_t m_nrPlayers;
	std::vector<Player> m_Players;
	std::stack<Card> m_Deck;
	Deck m_AscendingDeckFirst;
	Deck m_AscendingDeckSecond;
	Deck m_DescendingDeckFirst;
	Deck m_DescendingDeckSecond;

	unsigned int m_time;
};

