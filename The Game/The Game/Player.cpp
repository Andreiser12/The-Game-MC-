#include "Player.h"

Player::Player(const std::vector<Cards>& cards,
	const UserProfile& userprofile) :
	m_cards(cards), m_userprofile(userprofile){}

Player::~Player()
{
}

void Player::PlaceCards()
{
	unsigned int cardsPlaced = 0;
	while (GetCardsCount() && cardsPlaced < 2u)
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
		RemoveCard(cardToPlace);
		++cardsPlaced;
	}
}

void Player::PrintCards()
{
	for (Cards card : m_cards)
	{
		std::cout << card;
	}
	std::cout << std::endl;
}

void Player::DrawCards()
{
}

bool Player::FindCard(unsigned int id) const
{
	for (Cards card : m_cards)
	{
		if (id == card.getId()) return true;
	}
	return false;
}

void Player::RemoveCard(unsigned int id)
{
	std::vector<Cards> updatedCards;
	for (Cards card : m_cards)
	{
		if (id != card.getId())
		{
			updatedCards.push_back(card);
		}
	}
	SetCards(updatedCards);
}

void Player::SetCards(const std::vector<Cards>& cards)
{
	m_cards = cards;
}

const std::vector<Cards>& Player::GetCards() const
{
	return m_cards;
}

size_t Player::GetCardsCount() const
{
	return m_cards.size();
}
