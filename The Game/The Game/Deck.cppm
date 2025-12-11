export module Deck;
import Card;
import FundamentalCard;

import <iostream>;
import <stack>;

using std::uint8_t;
export using DeckStack = std::stack<Card<uint16_t>>;

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

	const DeckStack& GetDeck() const;
	DeckType GetDeckType() const;
	DeckNumber GetDeckNumber() const;
	const Card<uint16_t>& GetTopCard() const;

	void PushCard(const Card<uint16_t>& card);

public:
	constexpr static unsigned int ASCENDING_START_CARD = 1u;
	constexpr static unsigned int DESCENDING_START_CARD = 100u;
private:
	DeckStack m_deck;
	DeckType m_deckType : 1;
	DeckNumber m_deckNumber : 1;
};



