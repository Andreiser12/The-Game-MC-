#pragma once
#include <iostream>
#include <stack>
#include "Cards.h"
using std::uint8_t;

class Deck
{
public:

	enum class DeckType : uint8_t
	{
		Ascending,
		Descending
	};

	enum class DeckNumber : uint8_t
	{
		First,
		Second
	};

private:
	std::stack<Cards> m_deck;
	DeckType m_deckType : 1;
	DeckNumber m_deckNumber : 1;
};



