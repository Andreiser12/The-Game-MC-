module Deck;
#include<iostream>
#include<stack>

Deck::Deck(DeckType deckType, DeckNumber deckNumber) :
    m_deckType(deckType), m_deckNumber(deckNumber)
{
    if (m_deckType == DeckType::Ascending)
    {
        m_deck.push(FundamentalCard(1));
    }
    else
    {
        m_deck.push(FundamentalCard(100));
    }
}

const std::stack<Card>& Deck::GetDeck() const
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

const Card& Deck::GetTopCard() const
{
    return m_deck.top();
}

void Deck::PushCard(const Card& card)
{
    m_deck.push(card);
}