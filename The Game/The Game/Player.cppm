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
	Player(unsigned playerID, const std::string& name);
	Player(unsigned playerID, const std::unordered_set<Card<uint16_t>, CardsSetHashFunction<uint16_t>,
		CardsEqual<uint16_t>>& cards,
		const UserProfile& userprofile);
	~Player();

	void PlaceCards(Deck& chosenDeck);
	void PrintCards() const;
	void DrawCards(std::stack<Card<uint16_t>>& deck);
	bool FindCard(uint16_t cardId) const;
	void RemoveCard(uint16_t cardId);

	const static inline bool IsCardValid(const Deck& chosenDeck, uint16_t cardID);
	void SetupCards(std::stack<Card<uint16_t>>& deck, uint8_t cardsCount);

	void SetCards(const std::unordered_set<Card<uint16_t>, CardsSetHashFunction<uint16_t>, CardsEqual<uint16_t>>& cards);

	unsigned GetPlayerID() const;
	const std::string& GetUsername() const;
	const std::unordered_set<Card<uint16_t>, CardsSetHashFunction<uint16_t>,
		CardsEqual<uint16_t>>& GetCards() const;
	size_t GetCardsCount() const;
	UserProfile& GetUserProfile();

	bool operator<(Player& other) const;

private:
	unsigned m_playerID;
	std::string m_username;
	UserProfile m_userprofile;
	std::unordered_set<Card<uint16_t>, CardsSetHashFunction<uint16_t>, CardsEqual<uint16_t>> m_cards;
	uint8_t m_cardsPlacedRound=0;
};

