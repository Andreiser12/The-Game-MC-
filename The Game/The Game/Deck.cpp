module Deck;

Deck::Deck(DeckType deckType, DeckNumber deckNumber, uint8_t deckIndex) :
    m_deckType{ deckType }, m_deckNumber{ deckNumber }, m_deckIndex{ deckIndex }
{
    const uint8_t startValue = (deckType == DeckType::Ascending) ? ASCENDING_START_CARD : DESCENDING_START_CARD;
    m_pile.emplace(Card<uint8_t>(startValue));
}

const Pile& Deck::GetDeck() const
{
    return m_pile;
}

Deck::DeckType Deck::GetDeckType() const noexcept
{
    return m_deckType;
}

Deck::DeckNumber Deck::GetDeckNumber() const noexcept
{
    return m_deckNumber;
}

const Card<uint8_t>& Deck::GetTopCard() const
{
    return m_pile.top();
}

uint8_t Deck::GetDeckIndex() const noexcept
{
    return m_deckIndex;
}

void Deck::PushCard(const Card<uint8_t>& card)
{
    m_pile.emplace(card);
}