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

export struct HeuristicTree
{
	static int Evaluate(const AIMove& move, const Table& table, std::span<const Player> players);
	static int LocalScore(const AIMove& move);
	static int BackwardsBonus(const AIMove& move);
	static int GlobalPenalty(const AIMove& move, const Table& table, std::span<const Player> players);
	static int RiskPenalty(const AIMove& move, const Table& table);
};


export class GreedyAI
{
public:
	[[nodiscard]] static std::optional<AIMove>FindBestMove(const Player& currentPlayer, Table& table);

	static void PlayTurn(Player& currentPlayer, Table& table, uint8_t minCards);
};