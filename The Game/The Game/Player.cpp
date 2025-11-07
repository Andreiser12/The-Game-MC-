#include "Player.h"

constexpr static unsigned int kMinimumCards = 2u;

Player::Player(const std::vector<Cards>& cards,
	const UserProfile& userprofile) :
	m_cards(cards), m_userprofile(userprofile){}

Player::~Player()
{
}

void Player::PlaceCards(std::stack<Cards>& m_AscendingDeckFirst,
						std::stack<Cards>& m_AscendingDeckSecond,
						std::stack<Cards>& m_DescendingDeckFirst,
						std::stack<Cards>& m_DescendingDeckSecond)
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
		std::cout << "In which stack would you like to place the card? (Ascending/Descending)"
			<< std::endl;
		std::string choice;
		std::cin >> choice;
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
		m_cards.push_back(card);
		--m_cardsPlacedRound;
	}
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
