module GreedyAI;

static constexpr int LOCAL_WEIGHT = 1;
static constexpr int GLOBAL_WEIGHT = 2;
static constexpr int RISK_WEIGHT = 1;



std::optional<AIMove>GreedyAI::FindBestMove(const Player& currentPlayer, Table& table)
{
	std::vector<AIMove> moves;// verificam toate u_set

	auto scoreMove = [&](const Deck& deck, const Card<uint16_t>& card) {
		auto topCard = deck.GetTopCard().getId();
		return std::abs(topCard - card.getId());
		};

	for (auto& card : currentPlayer.GetCards())
	{
		for (auto& deck : table.GetAllDecks())
		{
			if (table.CheckCard(*deck, card))
			{
				moves.push_back({ deck, card, scoreMove(*deck, card) });
			}
		}
	}

	if (moves.empty())
	{
		return std::nullopt;
	}
	//auto bestMove = std::ranges::min_element(moves, {}, [](const AIMove& move) {return move.score; });
	auto bestMove = std::ranges::min_element(moves, {}, &AIMove::score);

	return *bestMove;
}

void GreedyAI::PlayTurn(Player& currentPlayer, Table& table, uint8_t minCards)
{

}

int HeuristicTree::Evaluate(const AIMove& move, const Table& table, std::span<const Player> players)
{
	int score = 0;

	score += LOCAL_WEIGHT * LocalScore(move)
		- BackwardsBonus(move)
		+ GLOBAL_WEIGHT * GlobalPenalty(move, table, players)
		+ RISK_WEIGHT * RiskPenalty(move, table);

	return score;
}

int HeuristicTree::LocalScore(const AIMove& move)
{
	auto topCard = static_cast<int>(move.deck->GetTopCard().getId());
	auto cardId = static_cast<int>(move.card.getId());
	return (std::abs(cardId - topCard));

}
int HeuristicTree::BackwardsBonus(const AIMove& move)
{
	auto topCard = static_cast<int>(move.deck->GetTopCard().getId());
	auto cardId = static_cast<int>(move.card.getId());

	if (std::abs(cardId - topCard) == 10)
	{
		return 10;
	}

	return 0;
}

int HeuristicTree::GlobalPenalty(const AIMove& move, const Table& table, std::span<const Player> players)
{
	int penalty = 0;

	for (const auto& player : players)
	{
		int validMoves = 0;

		for (const auto& card : player.GetCards())
		{
			if (table.CheckCard(*move.deck, card))
			{
				validMoves++;
			}
		}

		if (validMoves == 0)
		{
			penalty += 50;
		}
		else if (validMoves <= 2)
		{
			penalty += 20;
		}
		else if (validMoves <= 5)
		{
			penalty += 10;
		}
	}

	return penalty;
}

int HeuristicTree::RiskPenalty(const AIMove& move, const Table& table)
{
	int topCard = static_cast<int>(move.deck->GetTopCard().getId());
	int cardId = static_cast<int>(move.card.getId());
	int gap = std::abs(cardId - topCard);

	if (gap >= 30)
	{
		return 20;
	}
	else if (gap >= 20)
	{
		return 10;
	}

	return 0;

}