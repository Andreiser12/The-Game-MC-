export module Deck;
import Card;
import FundamentalCard;

import <stack>;

using std::uint8_t;
export using Pile = std::stack<Card<uint8_t>>;

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
	Deck(DeckType deckType, DeckNumber deckNumber, uint8_t deckIndex);

	const Pile& GetDeck() const;
	DeckType GetDeckType() const;
	DeckNumber GetDeckNumber() const;
	const Card<uint8_t>& GetTopCard() const;
	uint8_t GetDeckIndex() const;

	void PushCard(const Card<uint8_t>& card);

public:
	constexpr static uint8_t ASCENDING_START_CARD{ 1u };
	constexpr static uint8_t DESCENDING_START_CARD{ 100u };

private:
	Deck() = delete;

private:
	Pile m_pile;
	DeckType m_deckType : 1;
	DeckNumber m_deckNumber : 1;
	uint8_t m_deckIndex;
};



