export module Player;

import <stack>;
import <unordered_set>;
import <cstddef>;
import <string>;
import <functional>;

import Card;
import Deck;

export using PlayerHand = std::unordered_set<Card<uint8_t>,
	CardsSetHashFunction<uint8_t>, CardsEqual<uint8_t>>;

export class Player
{
public:
	Player(unsigned playerID, const std::string& username);
	~Player();

	void AddCard(const Card<uint8_t>& card);
	void DrawCards(Pile& deck);
	bool FindCard(uint8_t cardId) const;
	void RemoveCard(uint8_t cardId);

	void SetupCards(Pile& deck, uint8_t cardsCount);
	void IncrementCardsPlaced();
	void ResetCardsPlaced();

	void SetTimeOut(bool value);
	void SetIncative(bool value);

	unsigned GetPlayerId() const;
	const std::string& GetUsername() const;
	const PlayerHand& GetCards() const;
	size_t GetCardsCount() const;
	uint8_t GetCardsPlacedRound() const;

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

