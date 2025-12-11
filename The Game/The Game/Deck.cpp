module Deck;

Deck::Deck(DeckType deckType, DeckNumber deckNumber) :
    m_deckType(deckType), m_deckNumber(deckNumber)
{
    if (m_deckType == DeckType::Ascending)
    {
        m_deck.push(FundamentalCard(ASCENDING_START_CARD));
    }
    else
    {
        m_deck.push(FundamentalCard(DESCENDING_START_CARD));
    }
}

const DeckStack& Deck::GetDeck() const
{
    return m_deck;
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
    return m_deck.top();
}

void Deck::PushCard(const Card<uint16_t>& card)
{
    m_deck.push(card);
}