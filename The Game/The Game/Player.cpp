module Player;
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

constexpr static uint8_t MINIMUM_CARDS = 2;

Player::Player(const std::unordered_set<Card, Card::CardsSetHashFunction,
	Card::CardsEqual>& cards,
	const UserProfile& userprofile) :
	m_cards{ cards }, m_userprofile{ userprofile } {
}

Player::~Player() {}

void Player::PlaceCards(Deck& chosenDeck)
{
	uint8_t cardsPlaced = 0;
	while (GetCardsCount() && cardsPlaced < MINIMUM_CARDS)
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
		if (!IsCardValid(chosenDeck, cardToPlace))
		{
			std::cout << "You can't place this card on this deck!"
				<< std::endl;
			continue;
		}
		RemoveCard(cardToPlace);
		++cardsPlaced;
	}
	m_cardsPlacedRound = cardsPlaced;
}

void Player::PrintCards() const
{
	for (Card card : m_cards)
	{
		std::cout << card;
	}
	std::cout << std::endl;
}

void Player::DrawCards(std::stack<Card>& deck)
{
	while (!deck.empty() && m_cardsPlacedRound)
	{
		Card card = deck.top();
		deck.pop();
		m_cards.emplace(card);
		--m_cardsPlacedRound;
	}
}

bool Player::FindCard(unsigned int id) const
{
	Card cardToFind = Card(id);
	return m_cards.find(cardToFind) != m_cards.end();
}

void Player::RemoveCard(unsigned int id)
{
	Card cardToDelete(id);
	m_cards.erase(cardToDelete);
}

inline const bool Player::IsCardValid(const Deck& chosenDeck, unsigned int cardID)
{
	return (chosenDeck.GetDeckType() == Deck::DeckType::Ascending
		&& cardID > chosenDeck.GetTopCard().getId())
		|| (chosenDeck.GetDeckType() == Deck::DeckType::Descending
			&& cardID < chosenDeck.GetTopCard().getId());
}

void Player::SetupCards(std::stack<Card>& deck, uint8_t cardsCount)
{
	Card cardToPlace;
	for (uint8_t i = 0; i < cardsCount; i++)
	{
		cardToPlace = deck.top();
		deck.pop();
		m_cards.emplace(cardToPlace);
	}
}

void Player::SetCards(const std::unordered_set<Card, Card::CardsSetHashFunction,
	Card::CardsEqual>& cards)
{
	m_cards = cards;
}

const std::unordered_set<Card, Card::CardsSetHashFunction,
	Card::CardsEqual>& Player::GetCards() const
{
	return m_cards;
}

size_t Player::GetCardsCount() const
{
	return m_cards.size();
}

UserProfile& Player::GetUserProfile()
{
	return m_userprofile;
}

bool Player::operator<(Player& other) const
{
	if (m_userprofile.PerformanceScore() == other.GetUserProfile().PerformanceScore())
	{
		return m_userprofile.GetUsername() < other.GetUserProfile().GetUsername();
	}
	return m_userprofile.PerformanceScore() > other.GetUserProfile().PerformanceScore();
}
