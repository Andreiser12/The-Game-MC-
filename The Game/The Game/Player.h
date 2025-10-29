#pragma once
#include <unordered_set>
#include "Cards.h"

class Player
{
public:
	Player(const std::unordered_set<Cards>& cards);
	~Player();
	void PlaceCards();
	void PrintCards();
	void DrawCards();

	const std::unordered_set<Cards> GetCards() const;

private:
	//UserProfile m_userProfile;
	std::unordered_set<Cards> m_cards;
};

