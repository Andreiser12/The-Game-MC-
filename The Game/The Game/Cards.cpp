#include "Cards.h"

Cards::Cards(unsigned int id) : m_id(id) {}

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
