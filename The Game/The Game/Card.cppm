export module Card;

import <iostream>;
import <concepts>;
import <cstdint>;
import <functional>; 

export template<std::integral Integer>
class Card
{
public:
	Card() = default;
	Card(Integer id);

	Integer getId() const;

	bool operator == (const Card<Integer>& other) const;

	template<std::integral I>
	friend std::ostream& operator<<(std::ostream& out, const Card<I>& card);

protected:
	Integer m_id{};

};

export template<std::integral Integer>
struct CardsSetHashFunction
{
	std::size_t operator()(const Card<Integer>& card) const noexcept
	{
		std::size_t hash = std::hash<Integer>()(card.getId());
		return hash;
	}
};


export template<std::integral Integer>
struct CardsEqual
{
	bool operator()(const Card<Integer>& c1, const Card<Integer>& c2) const
	{
		return c1.getId() == c2.getId();
	}
};

