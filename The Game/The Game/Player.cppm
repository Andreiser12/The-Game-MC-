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

export using PlayerHand = std::unordered_set<Card<uint16_t>, CardsSetHashFunction<uint16_t>, CardsEqual<uint16_t>>;

export class Player
{
public:
	Player(unsigned playerID, const std::string& name);
	Player(unsigned playerID, const PlayerHand& cards,
		const UserProfile& userprofile);
	~Player();

	void PlaceCards(Deck& chosenDeck);
	void PrintCards() const;
	void DrawCards(DeckStack& deck);
	bool FindCard(uint16_t cardId) const;
	void RemoveCard(uint16_t cardId);

	const static inline bool IsCardValid(const Deck& chosenDeck, uint16_t cardID);
	void SetupCards(DeckStack& deck, uint8_t cardsCount);

	void SetCards(const PlayerHand& cards);

	void SetTimeOut(bool value);
	void SetIncative(bool value);

	unsigned GetPlayerID() const;
	const std::string& GetUsername() const;
	const PlayerHand& GetCards() const;
	size_t GetCardsCount() const;
	UserProfile& GetUserProfile();

	bool operator<(Player& other) const;

	bool HasTimeOut() const;
	bool IsInactive() const;

	

private:
	unsigned m_playerID;
	std::string m_username;
	UserProfile m_userprofile;
	PlayerHand m_cards;
	uint8_t m_cardsPlacedRound{ 0 };

	bool m_hasTimeOut{ false };
	bool m_isInactive{ false };
};

