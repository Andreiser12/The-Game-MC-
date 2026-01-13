module Card;

template<std::integral Integer>
constexpr Card<Integer>::Card(Integer id) noexcept :
	m_id{ id } {}

template<std::integral Integer>
constexpr Integer Card<Integer>::GetId() const noexcept
{
	return m_id;
}

template class Card<std::uint8_t>;
