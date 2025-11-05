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

const std::vector<Cards>& Player::GetCards() const
{
	return m_cards;
}

size_t Player::GetCardsCount() const
{
	return m_cards.size();
}
