export module GreedyAI;

import <optional>;
import <vector>;
import <ranges>;
import <cmath>;
import <set>;

import Card;
import Deck;
import Player;
import Table;

export struct AIMove
{
	Deck* deck;
	Card<uint16_t> card;
	int score;
};

export class GreedyAI
{
public:
	[[nodiscard]] static std::optional<AIMove>FindBestMove(const Player& currentPlayer, Table& table);

	static void PlayTurn(Player& currentPlayer, Table& table, uint8_t minCards);
};