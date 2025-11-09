#pragma once
#include <iostream>

class Cards
{
public:
	enum class Color : uint8_t
	{
		Gray,
		Blue
	};
protected:
	unsigned int m_id;
	Color m_color;
public:
	Cards(unsigned int id);
	unsigned int getId() const;
	Color getColor() const;

	friend std::ostream& operator<<(std::ostream& out, const Cards& card);
};

