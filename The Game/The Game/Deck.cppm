export module Deck;
import Card;
import FundamentalCard;

import <iostream>;
import <stack>;

using std::uint8_t;

export class Deck
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

public:
	Deck() = default;
	Deck(DeckType deckType, DeckNumber deckNumber);

	const std::stack<Card>& GetDeck() const;
	DeckType GetDeckType() const;
	DeckNumber GetDeckNumber() const;
	const Card& GetTopCard() const;

	void PushCard(const Card& card);


private:
	std::stack<Card> m_deck;
	DeckType m_deckType : 1;
	DeckNumber m_deckNumber : 1;
};



