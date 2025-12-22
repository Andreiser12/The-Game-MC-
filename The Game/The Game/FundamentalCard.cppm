export module FundamentalCard;

import Card;

export class FundamentalCard : public Card<std::uint8_t>
{
public:
	FundamentalCard(uint8_t id);
};
