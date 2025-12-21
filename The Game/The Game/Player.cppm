export module Player;

import <iostream>;
import <vector>;
import <stack>;
import <unordered_set>;
import <cstddef>;
import <functional>;

import Card;
import Deck;

export using PlayerHand = std::unordered_set<Card<uint16_t>,
	CardsSetHashFunction<uint16_t>, CardsEqual<uint16_t>>;

export class Player
{
public:
	Player(unsigned playerID, const std::string& username);
	Player(unsigned playerID, const PlayerHand& cards);
	~Player();

	void PlaceCards(Deck& chosenDeck);
	void PrintCards() const;
	void DrawCards(Pile& deck);
	bool FindCard(uint16_t cardId) const;
	void RemoveCard(uint16_t cardId);

	const static inline bool IsCardValid(const Deck& chosenDeck, uint16_t cardId);
	void SetupCards(Pile& deck, uint8_t cardsCount);

	void SetTimeOut(bool value);
	void SetIncative(bool value);

	unsigned GetPlayerId() const;
	const std::string& GetUsername() const;
	const PlayerHand& GetCards() const;
	size_t GetCardsCount() const;

	bool operator<(Player& other) const;

	bool HasTimeOut() const;
	bool IsInactive() const;

private:
	Player() = delete;

private:
	unsigned m_playerId;
	std::string m_username;
	PlayerHand m_cards;
	uint8_t m_cardsPlacedRound{ 0u };

	bool m_hasTimeOut{ false };
	bool m_isInactive{ false };
};

