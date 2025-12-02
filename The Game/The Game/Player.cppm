export module Player;

import <iostream>;
import <vector>;
import <stack>;
import <unordered_set>;

import Card;
import UserProfile;
import Deck;

export class Player
{
public:
	Player(unsigned int playerID, const std::unordered_set<Card, Card::CardsSetHashFunction,
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

	unsigned int GetPlayerID() const;
	const std::unordered_set<Card, Card::CardsSetHashFunction,
		Card::CardsEqual>& GetCards() const;
	size_t GetCardsCount() const;
	UserProfile& GetUserProfile();

	bool operator<(Player& other) const;

private:
	unsigned int m_playerID;
	UserProfile m_userprofile;
	std::unordered_set<Card, Card::CardsSetHashFunction, Card::CardsEqual> m_cards;
	unsigned int m_cardsPlacedRound=0;
};

