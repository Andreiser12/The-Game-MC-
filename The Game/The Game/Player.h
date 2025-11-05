#pragma once
#include <iostream>
#include <vector>
#include "Cards.h"
#include "UserProfile.h"

class Player
{
public:
	Player(const std::vector<Cards>& cards,
		const UserProfile& userprofile);
	~Player();
	void PlaceCards();
	void PrintCards();
	void DrawCards();
	bool FindCard(unsigned int id) const;
	void RemoveCard(unsigned int id);

	void SetCards(const std::vector<Cards>& cards);

	const std::vector<Cards>& GetCards() const;
	size_t GetCardsCount() const;

private:
	UserProfile m_userprofile;
	std::vector<Cards> m_cards;
};

