export module Deck;
import Card;

import <stack>;
import <utility>;

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

public:
	Deck(DeckType deckType, uint8_t deckIndex);

	DeckType GetDeckType() const noexcept;
	const Card<uint8_t>& GetTopCard() const;
	uint8_t GetDeckIndex() const noexcept;
	uint8_t GetDeckSize() const noexcept;

	template<typename CardType>
	void PlaceCard(CardType&& card)
	{
		m_pile.emplace(std::forward<CardType>(card));
	};

public:
	constexpr static uint8_t ASCENDING_START_CARD{ 1u };
	constexpr static uint8_t DESCENDING_START_CARD{ 100u };

private:
	Deck() = delete;

private:
	Pile m_pile;
	DeckType m_deckType : 1;
	uint8_t m_deckIndex;
};
