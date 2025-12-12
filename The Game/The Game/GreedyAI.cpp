module GreedyAI;


std::optional<AIMove>GreedyAI::FindBestMove(const Player& currentPlayer, Table& table)
{
	std::vector<AIMove> moves;

	auto scoreMove = [&](Deck& deck, Card<uint16_t>& card) {
		auto topCard = deck.GetTopCard().getId();
		return std::abs(topCard - card.getId());
		};

	/*for (auto& card : currentPlayer.GetCards())
	{
		for (auto& deck : table.getAllDecks())
		{
			if (table.CheckCard(deck, card))
			{
				moves.push_back(&deck, card, scoreMove);
			}
		}
	}*/

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