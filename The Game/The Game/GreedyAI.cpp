//module GreedyAI;
//
//static constexpr int LOCAL_WEIGHT{ 1 };
//static constexpr int GLOBAL_WEIGHT{ 2 };
//static constexpr int RISK_WEIGHT{ 1 };
//
//std::optional<AIMove>GreedyAI::FindBestMove(const Player& currentPlayer, Table& table)
//{
//	std::vector<AIMove> moves;
//
//	for (const auto& card : currentPlayer.GetCards())
//	{
//		for (const auto& deckReference : table.GetAllDecks())
//		{
//			auto& deck{ deckReference };
//			if (table.PlayCard(currentPlayer.GetPlayerId(), deck->GetDeckIndex(), card))
//			{
//				AIMove move{ *deck, card, 0 };
//				move.score = HeuristicTree::Evaluate(move, table, table.GetPlayers());
//				moves.push_back(move);
//			}
//		}
//	}
//
//	if (moves.empty()) return std::nullopt;
//
//	auto bestMove{ std::ranges::min_element(moves, {}, &AIMove::score) };
//	return *bestMove;
//}
//
//void GreedyAI::PlayTurn(Player& currentPlayer, Table& table, uint8_t minCards)
//{
//	for (size_t i = 0; i < minCards; i++)
//	{
//		auto bestMove{ FindBestMove(currentPlayer, table) };
//		if (!bestMove) break;
//		table.PlaceCardOnDeck(bestMove.value().deck.get().GetDeckIndex(), bestMove.value().card);
//		currentPlayer.RemoveCard(bestMove.value().card.GetId());
//	}
//}
//
//int HeuristicTree::Evaluate(const AIMove& move, const Table& table, const std::unordered_map<unsigned, Player>& players)
//{
//	int score{ 0 };
//	score += LOCAL_WEIGHT * LocalScore(move)
//		- BackwardsBonus(move)
//		+ GLOBAL_WEIGHT * GlobalPenalty(move, table, players)
//		+ RISK_WEIGHT * RiskPenalty(move, table);
//	return score;
//}
//
//int HeuristicTree::LocalScore(const AIMove& move)
//{
//	auto topCard{ static_cast<int>(move.deck.get().GetTopCard().GetId()) };
//	auto cardId{ static_cast<int>(move.card.GetId()) };
//	return (std::abs(cardId - topCard));
//}
//
//int HeuristicTree::BackwardsBonus(const AIMove& move)
//{
//	auto topCard{ static_cast<int>(move.deck.get().GetTopCard().GetId()) };
//	auto cardId{ static_cast<int>(move.card.GetId()) };
//
//	if (std::abs(cardId - topCard) == 10)
//	{
//		return 10;
//	}
//	return 0;
//}
//
//int HeuristicTree::GlobalPenalty(const AIMove& move, const Table& table,
//	const std::unordered_map<unsigned, Player>& players)
//{
//	int penalty{ 0 };
//
//	for (const auto& [id, player] : players)
//	{
//		int validMoves{ 0 };
//
//		for (const auto& card : player.GetCards())
//		{
//			if (table.PlayCard(player.GetPlayerId(), move.deck.get().GetDeckIndex(), card))
//			{
//				validMoves++;
//			}
//		}
//
//		if (validMoves == 0)
//		{
//			penalty += 50;
//		}
//		else if (validMoves <= 2)
//		{
//			penalty += 20;
//		}
//		else if (validMoves <= 5)
//		{
//			penalty += 10;
//		}
//	}
//	return penalty;
//}
//
//int HeuristicTree::RiskPenalty(const AIMove& move, const Table& table)
//{
//	int topCard{ static_cast<int>(move.deck.get().GetTopCard().GetId()) };
//	int cardId{ static_cast<int>(move.card.GetId()) };
//	int gap{ std::abs(cardId - topCard) };
//
//	if (gap >= 30)
//	{
//		return 20;
//	}
//	else if (gap >= 20)
//	{
//		return 10;
//	}
//	return 0;
//}