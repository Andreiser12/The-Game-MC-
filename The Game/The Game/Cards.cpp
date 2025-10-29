#include "Cards.h"

Cards::Cards(unsigned int id) : m_id(id) {}

unsigned int Cards::getId() const {
	return m_id;
}
