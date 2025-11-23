export module Player;
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
import Card;
import UserProfile;
import Deck;

export class Player
{
public:
	Player(const std::unordered_set<Card, Card::CardsSetHashFunction,
		Card::CardsEqual>& cards,
		const UserProfile& userprofile);
	~Player();
	void PlaceCards(Deck& chosenDeck);
	void PrintCards() const;
	void DrawCards(std::stack<Card>& deck);
	bool FindCard(unsigned int id) const;
	void RemoveCard(unsigned int id);
	const static inline bool IsCardValid(const Deck& chosenDeck, unsigned int cardID);
	void SetupCards(std::stack<Card>& deck, uint8_t cardsCount);

	void SetCards(const std::unordered_set<Card, Card::CardsSetHashFunction,
		Card::CardsEqual>& cards);

	const std::unordered_set<Card, Card::CardsSetHashFunction,
		Card::CardsEqual>& GetCards() const;
	size_t GetCardsCount() const;
	UserProfile& GetUserProfile();

	bool operator<(Player& other) const;

private:
	UserProfile m_userprofile;
	std::unordered_set<Card, Card::CardsSetHashFunction, Card::CardsEqual> m_cards;
	unsigned int m_cardsPlacedRound=0;
};

