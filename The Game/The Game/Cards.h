#pragma once
#include <iostream>

class Cards
{
protected:
	unsigned int m_id;
public:
	Cards(unsigned int id);
	unsigned int getId() const;

	friend std::ostream& operator<<(std::ostream& out, const Cards& card);
};

