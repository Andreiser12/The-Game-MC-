export module FundamentalCard;

import Card;

export class FundamentalCard : public Card<std::uint16_t>
{
public:
	FundamentalCard(uint16_t id);
};
