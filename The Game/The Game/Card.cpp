module Card;

template<std::integral Integer>
Card<Integer>::Card(Integer id) : m_id{ id } {}

template<std::integral Integer>
Integer Card<Integer>::getId() const
{
	return m_id;
}

template <std::integral I>
std::ostream& operator<<(std::ostream& out, const Card<I>& card)
{
	out << card.getId() << std::endl;
	return out;
}

template<std::integral Integer>
bool Card<Integer>::operator == (const Card<Integer>& other) const
{
	return m_id == other.getId();
}

template class Card<std::uint16_t>;
template std::ostream& operator<<(std::ostream&, const Card<std::uint16_t>&);
template bool Card<std::uint16_t>::operator==(const Card<std::uint16_t>&) const;
