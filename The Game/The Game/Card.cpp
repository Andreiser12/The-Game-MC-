module Card;

template<std::integral Integer>
Card<Integer>::Card(Integer id) : m_id{ id } {}

template<std::integral Integer>
Integer Card<Integer>::GetId() const
{
	return m_id;
}

template<std::integral Integer>
bool Card<Integer>::operator == (const Card<Integer>& other) const
{
	return m_id == other.GetId();
}

template class Card<std::uint8_t>;
template bool Card<std::uint8_t>::operator==(const Card<std::uint8_t>&) const;
