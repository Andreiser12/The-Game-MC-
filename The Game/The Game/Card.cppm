export module Card;

import <concepts>;
import <cstdint>;
import <functional>; 

export template<std::integral Integer>
class Card
{
public:
	explicit constexpr Card(Integer id) noexcept;

	constexpr Integer GetId() const noexcept;

	constexpr bool operator == (const Card&) const noexcept = default;

private:
	Card() = delete;

private:
	Integer m_id{};

};

namespace std
{
	template<std::integral Integer>
	struct hash<Card<Integer>> 
	{
		constexpr size_t operator()(const Card<Integer>& card) const noexcept
		{
			return hash<std::uint8_t>{}(card.GetId());
		}
	};
}

