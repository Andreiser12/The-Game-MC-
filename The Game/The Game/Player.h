#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include "Cards.h"
#include "UserProfile.h"
#include "Deck.h"

class Player
{
public:
	Player(const std::vector<Cards>& cards,
		const UserProfile& userprofile);
	~Player();
	void PlaceCards(Deck& chosenDeck);
	void PrintCards() const;
	void DrawCards(std::stack<Cards>& deck);
	bool FindCard(unsigned int id) const;
	void RemoveCard(unsigned int id);
	const static inline bool IsCardValid(const Deck& chosenDeck, unsigned int cardID);

	void SetCards(const std::vector<Cards>& cards);

	const std::vector<Cards>& GetCards() const;
	size_t GetCardsCount() const;

private:
	UserProfile m_userprofile;
	std::vector<Cards> m_cards;
	unsigned int m_cardsPlacedRound=0;
};

