#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

#include "Card.h"
#include "UserProfile.h"
#include "Deck.h"

class Player
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

	void SetCards(const std::unordered_set<Card, Card::CardsSetHashFunction,
		Card::CardsEqual>& cards);

	const std::unordered_set<Card, Card::CardsSetHashFunction,
		Card::CardsEqual>& GetCards() const;
	size_t GetCardsCount() const;
	UserProfile& GetUserProfile();

private:
	UserProfile m_userprofile;
	std::unordered_set<Card, Card::CardsSetHashFunction, Card::CardsEqual> m_cards;
	unsigned int m_cardsPlacedRound=0;
};

