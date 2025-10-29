#pragma once
#include <unordered_set>
#include "Cards.h"

class Player
{
public:
	Player(unsigned int finalCards=0, const std::unordered_set<Cards>& cards);
	~Player();
	void PlaceCards();
	void PrintCards() const;
	void DrawCards();

	unsigned int GetFinalCards() const;
	const std::unordered_set<Cards> GetCards() const;

private:
	unsigned int m_finalCards;
	//UserProfile m_userProfile;
	std::unordered_set<Cards> m_cards;
};

