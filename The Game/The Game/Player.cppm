export module Player;

import <iostream>;
import <vector>;
import <stack>;
import <unordered_set>;
import <cstddef>;
import <functional>;

import Card;
import UserProfile;	
import Deck;

export class Player
{
public:
	Player(unsigned playerID, const std::unordered_set<Card, CardsSetHashFunction,
		CardsEqual>& cards,
		const UserProfile& userprofile);
	~Player();

	void PlaceCards(Deck& chosenDeck);
	void PrintCards() const;
	void DrawCards(std::stack<Card>& deck);
	bool FindCard(uint16_t cardId) const;
	void RemoveCard(uint16_t cardId);

	const static inline bool IsCardValid(const Deck& chosenDeck, uint16_t cardID);
	void SetupCards(std::stack<Card>& deck, uint8_t cardsCount);

	void SetCards(const std::unordered_set<Card, CardsSetHashFunction, CardsEqual>& cards);

	unsigned GetPlayerID() const;
	const std::unordered_set<Card, CardsSetHashFunction,
		CardsEqual>& GetCards() const;
	size_t GetCardsCount() const;
	UserProfile& GetUserProfile();

	bool operator<(Player& other) const;

private:
	unsigned m_playerID;
	UserProfile m_userprofile;
	std::unordered_set<Card, CardsSetHashFunction, CardsEqual> m_cards;
	uint8_t m_cardsPlacedRound=0;
};

