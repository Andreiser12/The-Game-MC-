#include "Cards.h"

Cards::Cards(unsigned int id, Color color) : m_id(id), m_color(color) {}

unsigned int Cards::getId() const {
	return m_id;
}

Cards::Color Cards::getColor() const
{
	return m_color;
}

std::ostream& operator<<(std::ostream& out, const Cards& card)
{
	out << card.getId() << std::endl;
	return out;
}

bool Cards::operator == (const Cards& other) const
{
	return m_id == other.getId() && m_color == other.getColor();
}
