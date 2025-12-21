module Deck;

Deck::Deck(DeckType deckType, DeckNumber deckNumber, uint8_t deckIndex) :
    m_deckType{ deckType }, m_deckNumber{ deckNumber }, m_deckIndex{ deckIndex }
{
    if (m_deckType == DeckType::Ascending)
    {
        m_pile.push(FundamentalCard(ASCENDING_START_CARD));
    }
    else
    {
        m_pile.push(FundamentalCard(DESCENDING_START_CARD));
    }
}

const Pile& Deck::GetDeck() const
{
    return m_pile;
}

Deck::DeckType Deck::GetDeckType() const
{
    return m_deckType;
}

Deck::DeckNumber Deck::GetDeckNumber() const
{
    return m_deckNumber;
}

const Card<uint16_t>& Deck::GetTopCard() const
{
    return m_pile.top();
}

uint8_t Deck::GetDeckIndex() const
{
    return m_deckIndex;
}

void Deck::PushCard(const Card<uint16_t>& card)
{
    m_pile.push(card);
}