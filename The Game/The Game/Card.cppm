export module Card;

import <concepts>;
import <cstdint>;
import <functional>; 

export template<std::integral Integer>
class Card
{
public:
	Card() = default;
	Card(Integer id);

	Integer GetId() const noexcept;

	bool operator == (const Card<Integer>& other) const;

protected:
	Integer m_id{};

};

export template<std::integral Integer>
struct CardsSetHashFunction
{
	std::size_t operator()(const Card<Integer>& card) const noexcept
	{
		std::size_t hash = std::hash<Integer>()(card.GetId());
		return hash;
	}
};

export template<std::integral Integer>
struct CardsEqual
{
	bool operator()(const Card<Integer>& c1, const Card<Integer>& c2) const noexcept
	{
		return c1.GetId() == c2.GetId();
	}
};

