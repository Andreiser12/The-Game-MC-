#include "Player.h"

constexpr static unsigned int kMinimumCards = 2u;

Player::Player(const std::unordered_set<Cards, Cards::CardsSetHashFunction,
	Cards::CardsEqual>& cards,
	const UserProfile& userprofile) :
	m_cards{ cards }, m_userprofile{ userprofile } {
}

Player::~Player() {}

void Player::PlaceCards(Deck& chosenDeck)
{
	unsigned int cardsPlaced = 0;
	while (GetCardsCount() && cardsPlaced < kMinimumCards)
	{
		std::cout << "Which card would you like to place?"
			<< std::endl;
		PrintCards();
		unsigned int cardToPlace;

		std::cin >> cardToPlace;
		while (!FindCard(cardToPlace))
		{
			std::cout << "This card doesn't exist." <<
				std::endl << "Pleace choose an existing card: ";
			std::cin >> cardToPlace;
		}
		int tries = 0;
		while (!IsCardValid(chosenDeck, cardToPlace) && tries!=3)
		{
			tries++;
			std::cout << "You can't place this card on this deck!";
		}
		RemoveCard(cardToPlace);
		++cardsPlaced;
	}
	m_cardsPlacedRound = cardsPlaced;
}

void Player::PrintCards() const
{
	for (Cards card : m_cards)
	{
		std::cout << card;
	}
	std::cout << std::endl;
}

void Player::DrawCards(std::stack<Cards>& deck)
{
	while (!deck.empty() && m_cardsPlacedRound)
	{
		Cards card = deck.top();
		deck.pop();
		m_cards.emplace(card);
		--m_cardsPlacedRound;
	}
}

bool Player::FindCard(unsigned int id) const
{
	Cards cardToFind = Cards(id);
	return m_cards.find(cardToFind) != m_cards.end();
}

void Player::RemoveCard(unsigned int id)
{
	Cards cardToDelete(id);
	m_cards.erase(cardToDelete);
}

inline const bool Player::IsCardValid(const Deck& chosenDeck, unsigned int cardID)
{
	return (chosenDeck.GetDeckType() == Deck::DeckType::Ascending
		&& cardID > chosenDeck.GetTopCard().getId())
		|| (chosenDeck.GetDeckType() == Deck::DeckType::Descending
			&& cardID < chosenDeck.GetTopCard().getId());
}

void Player::SetCards(const std::unordered_set<Cards, Cards::CardsSetHashFunction,
	Cards::CardsEqual>& cards)
{
	m_cards = cards;
}

const std::unordered_set<Cards, Cards::CardsSetHashFunction,
	Cards::CardsEqual>& Player::GetCards() const
{
	return m_cards;
}

size_t Player::GetCardsCount() const
{
	return m_cards.size();
}
