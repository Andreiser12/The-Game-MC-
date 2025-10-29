#include "Player.h"

Player::Player(const std::vector<Cards>& cards,
	const UserProfile& userprofile) :
	m_cards(cards), m_userprofile(userprofile){}

Player::~Player()
{
}

void Player::PlaceCards()
{
}

void Player::PrintCards()
{
	/*for (Cards card : m_cards)
	{
		std::cout << card << " ";
	}*/
}

void Player::DrawCards()
{
}

const std::vector<Cards> Player::GetCards() const
{
	return m_cards;
}
