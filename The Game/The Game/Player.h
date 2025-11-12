#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>

#include "Cards.h"
#include "UserProfile.h"
#include "Deck.h"

class Player
{
public:
	Player(const std::unordered_set<Cards, Cards::CardsSetHashFunction,
		Cards::CardsEqual>& cards,
		const UserProfile& userprofile);
	~Player();
	void PlaceCards(Deck& chosenDeck);
	void PrintCards() const;
	void DrawCards(std::stack<Cards>& deck);
	bool FindCard(unsigned int id) const;
	void RemoveCard(unsigned int id);
	const static inline bool IsCardValid(const Deck& chosenDeck, unsigned int cardID);

	void SetCards(const std::unordered_set<Cards, Cards::CardsSetHashFunction,
		Cards::CardsEqual>& cards);

	const std::unordered_set<Cards, Cards::CardsSetHashFunction,
		Cards::CardsEqual>& GetCards() const;
	size_t GetCardsCount() const;

private:
	UserProfile m_userprofile;
	std::unordered_set<Cards, Cards::CardsSetHashFunction, Cards::CardsEqual> m_cards;
	unsigned int m_cardsPlacedRound=0;
};

