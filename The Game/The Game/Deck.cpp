module Deck;

Deck::Deck(DeckType deckType, uint8_t deckIndex) :
	m_deckType{ deckType }, m_deckIndex{ deckIndex }
{
	const uint8_t startValue = (deckType == DeckType::Ascending)
		? ASCENDING_START_CARD : DESCENDING_START_CARD;
	m_pile.emplace(startValue);
}

Deck::DeckType Deck::GetDeckType() const noexcept
{
	return m_deckType;
}

const Card<uint8_t>& Deck::GetTopCard() const
{
	return m_pile.top();
}

uint8_t Deck::GetDeckIndex() const noexcept
{
	return m_deckIndex;
}

uint8_t Deck::GetDeckSize() const noexcept
{
	return static_cast<uint8_t>(m_pile.size()) - 1;
}
