export module Card;

import <iostream>;

export class Card
{
public:
	enum class Color : uint8_t
	{
		Gray,
		Blue
	};

protected:
	unsigned int m_id;
	Color m_color;

public:
	Card() = default;
	Card(unsigned int id, Color color = Color::Gray);

	unsigned int getId() const;
	Color getColor() const;

	bool operator == (const Card& other) const;
	friend std::ostream& operator<<(std::ostream& out, const Card& card);

	struct CardsSetHashFunction
	{
		std::size_t operator()(const Card& card) const noexcept
		{
			std::size_t h1 = std::hash<unsigned int>()(card.m_id);
			std::size_t h2 = std::hash<uint8_t>()(static_cast<uint8_t>(card.m_color));
			return h1 ^ (h2 << 1);
		}
	};

	struct CardsEqual
	{
		bool operator () (const Card& c1, const Card& c2) const 
		{
			return c1.getId() == c2.getId() && c1.getColor() == c2.getColor();
		}
	};
};

