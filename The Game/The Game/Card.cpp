module Card;

Card::Card(uint16_t id, Color color) : m_id(id), m_color(color) {}

uint16_t Card::getId() const
{
	return m_id;
}

Card::Color Card::getColor() const
{
	return m_color;
}

std::ostream& operator<<(std::ostream& out, const Card& card)
{
	out << card.getId() << std::endl;
	return out;
}

bool Card::operator == (const Card& other) const
{
	return m_id == other.getId() && m_color == other.getColor();
}
