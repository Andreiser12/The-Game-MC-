module Table;

import GreedyAI;

constexpr static uint8_t DECK_COUNT{ 4u };

constexpr static uint8_t TWO_PLAYERS{ 2u };
constexpr static uint8_t CARDS_TWO_PLAYERS{ 8u };

constexpr static uint8_t THREE_PLAYERS{ 3u };
constexpr static uint8_t CARDS_THREE_PLAYERS{ 7u };

constexpr static uint8_t CARDS_DEFAULT_PLAYERS{ 6u };

constexpr static uint8_t MIN_CARDS_NORMAL{ 2u };
constexpr static uint8_t MIN_CARDS_EMPTY_DECK{ 1u };

AssistantAI::DeckType ConvertDeckType(Deck::DeckType type)
{
	if (type == Deck::DeckType::Ascending) return AssistantAI::DeckType::Ascending;
	return AssistantAI::DeckType::Descending;
}

Table::Table(std::span<const std::pair<unsigned, std::string_view>> lobbyPlayers) :
	m_playerCount{ static_cast<uint8_t>(lobbyPlayers.size()) }
{
	m_decks = {
			std::make_unique<Deck>(Deck::DeckType::Ascending, 0u),
	std::make_unique<Deck>(Deck::DeckType::Ascending, 1u),
	std::make_unique<Deck>(Deck::DeckType::Descending, 2u),
	std::make_unique<Deck>(Deck::DeckType::Descending, 3u)
	};

	Shuffle();
	m_players.reserve(lobbyPlayers.size());
	m_turnOrder.reserve(lobbyPlayers.size());

	size_t cardsPerPlayer;
	switch (m_playerCount)
	{
	case 2:
		cardsPerPlayer = 8;
		break;
	case 3:
		cardsPerPlayer = 7;
		break;
	case 4:
		cardsPerPlayer = 6;
		break;
	case 5:
		cardsPerPlayer = 6;
		break;
	default:
		cardsPerPlayer = 6;
		break;
	}
	for (const auto& [id, username] : lobbyPlayers)
	{
		auto [it, inserted] {m_players.try_emplace(id, id, username)};
		if (inserted)
		{
			it->second.SetupCards(m_pile, cardsPerPlayer);
			m_turnOrder.emplace_back(id);
		}
	}
}

bool Table::IsPileEmpty() const noexcept
{
	return m_pile.empty();
}

void Table::Shuffle()
{
	std::vector<Card<uint8_t>> cards;
	cards.reserve(98);

	for (uint8_t i = 2u; i < 100u; ++i)
		cards.emplace_back(i);

	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(cards.begin(), cards.end(), generator);

	for (auto& card : cards)
	{
		m_pile.emplace(std::move(card));
	}
}

bool Table::BackwardsTrick(uint8_t deckId, const Card<uint8_t>& card) const
{
	const uint8_t cardValue{ card.GetId() };
	const uint8_t topCardValue{ m_decks[deckId]->GetTopCard().GetId() };

	if (m_decks[deckId]->GetDeckType() == Deck::DeckType::Ascending)
	{
		return (topCardValue >= 10 && topCardValue - 10 == cardValue);
	}
	return (topCardValue + 10 == cardValue);
}

bool Table::PlayCard(unsigned playerId, uint8_t deckId, const Card<uint8_t>& card) const
{
	if (playerId != GetCurrentPlayerId()) return false;

	const uint8_t cardValue{ card.GetId() };
	const uint8_t topCardValue{ m_decks[deckId]->GetTopCard().GetId() };
	const bool isBackwards{ BackwardsTrick(deckId, card) };

	if (m_decks[deckId]->GetDeckType() == Deck::DeckType::Ascending)
	{
		return cardValue > topCardValue || isBackwards;
	}
	return cardValue < topCardValue || isBackwards;
}

void Table::AdvanceTurn() noexcept
{
	if (m_turnOrder.empty()) return;

	Player& currentPlayer{ GetPlayer(GetCurrentPlayerId()) };

	if (currentPlayer.HasTimeOut())
	{
		RunBotTurn(currentPlayer.GetPlayerId());
	}

	uint8_t attempts{ 0 };
	do
	{
		m_currentTurnIndex = (m_currentTurnIndex + 1) % m_playerCount;
		++attempts;

		if (attempts >= m_playerCount) break;

		if (!IsPileEmpty()) break;

		if (GetPlayer(GetCurrentPlayerId()).GetCardsCount() > 0) break;

	} while (true);
}

bool Table::HasPlayer(unsigned playerId) const
{
	return m_players.contains(playerId);
}

void Table::RunBotTurn(unsigned playerId)
{
	Player& player{ GetPlayer(playerId) };
	uint8_t minCardsToPlay{ IsPileEmpty() ? MIN_CARDS_EMPTY_DECK : MIN_CARDS_NORMAL };
	int cardsPlayed{ 0 };
	bool canPlayMore{ true };

	while (canPlayMore)
	{
		std::vector<uint8_t> myHand;
		for (const auto& card : player.GetCards())
		{
			myHand.emplace_back(card.GetId());
		}

		std::vector<AssistantAI::DeckState> deckStates;
		auto allDecks{ GetAllDecks() };

		for (const auto& deckRef : allDecks) {
			AssistantAI::DeckState state;
			state.index = static_cast<int>(deckRef->GetDeckIndex());
			state.topCard = deckRef->GetTopCard().GetId();
			state.type = ConvertDeckType(deckRef->GetDeckType());
			deckStates.push_back(state);
		}

		std::vector<std::vector<uint8_t>> otherHands;
		for (const auto& [id, otherPlayer] : m_players)
		{
			if (id != playerId)
			{
				std::vector<uint8_t> hand;
				for (const auto& card : otherPlayer.GetCards())
				{
					hand.push_back(card.GetId());
				}
				otherHands.push_back(hand);
			}
		}

		auto resultOpt{ AssistantAI::GreedyBot::FindBestMove(myHand, deckStates, otherHands) };

		if (!resultOpt) canPlayMore = false;

		else
		{
			auto move = resultOpt.value();

			if (cardsPlayed >= minCardsToPlay && move.score > 5)
			{
				canPlayMore = false;
			}
			else
			{
				Card<uint8_t> cardObj(move.cardValue);
				PlaceCardOnDeck(static_cast<uint8_t>(move.deckIndex), cardObj);
				player.RemoveCard(move.cardValue);
				player.IncrementCardsPlaced();
				++cardsPlayed;
			}
		}

		if (player.GetCardsCount() == 0) canPlayMore = false;
	}
	EndTurn(playerId);
}

Deck& Table::GetDeck(uint8_t deckId) const
{
	return *m_decks[deckId];
}

std::span<const std::unique_ptr<Deck>> Table::GetAllDecks() const noexcept
{
	return m_decks;
}

const std::unordered_map<unsigned, Player>& Table::GetPlayers() const
{
	return m_players;
}

const Player& Table::GetPlayer(unsigned playerId) const
{
	auto it = m_players.find(playerId);
	if (it == m_players.end()) throw std::runtime_error("Player not found");
	return it->second;
}

Player& Table::GetPlayer(unsigned playerId)
{
	return const_cast<Player&>(
		static_cast<const Table&>(*this).GetPlayer(playerId));
}

unsigned Table::GetCurrentPlayerId() const
{
	if (m_currentTurnIndex >= m_turnOrder.size())
		throw std::runtime_error("Invalid turn index.");
	return m_turnOrder[m_currentTurnIndex];
}

bool Table::CanPlayerPlayMinCards(const Player& player, uint8_t minCards) const
{
	if (static_cast<uint8_t>(player.GetCardsCount()) < minCards) return false;
	uint8_t validCards{ 0u };

	for (const auto & card : player.GetCards())
	{
		for (size_t deckIndex = 0; deckIndex < DECK_COUNT; ++deckIndex)
		{
			if (PlayCard(player.GetPlayerId(), deckIndex, card))
			{
				if (++validCards >= minCards) return true;
			}
		}
	}
	return false;
}

void Table::PlaceCardOnDeck(uint8_t deckIndex, const Card<uint8_t>& card)
{
	m_decks[deckIndex]->PushCard(card);
}

bool Table::CheckWinCondition() const noexcept
{
	if (!IsPileEmpty()) return false;

	return std::ranges::all_of(m_players | std::views::values,
		[](const auto& player) {
			return player.GetCardsCount() == 0; });
}

bool Table::CheckLoseCondition() const
{
	const auto& currentPlayer = GetPlayer(GetCurrentPlayerId());
	uint8_t minCards{ m_pile.empty() ? 1u : 2u };
	return !CanPlayerPlayMinCards(currentPlayer, minCards);
}

std::vector<uint8_t> Table::EndTurn(unsigned playerId)
{
	if (playerId != GetCurrentPlayerId()) return {};

	Player& player{ GetPlayer(playerId) };
	const uint8_t cardsPlaced{ player.GetCardsPlacedRound() };
	uint8_t drawCount{ static_cast<uint8_t>
		(std::min<std::size_t>(cardsPlaced, m_pile.size())) };
	std::vector<uint8_t> drawnCards;
	drawnCards.reserve(drawCount);

	for (uint8_t i = 0; i < drawCount; ++i)
	{
		const uint8_t cardId{ m_pile.top().GetId() };
		m_pile.pop();
		player.AddCard(std::move(m_pile.top()));
		drawnCards.emplace_back(cardId);
	}
	player.ResetCardsPlaced();
	AdvanceTurn();
	return drawnCards;
}

bool Table::RemovePlayer(unsigned playerId) noexcept
{
	auto it = m_players.find(playerId);
	if (it == m_players.end()) return false;

	auto turnIt = std::ranges::find(m_turnOrder, playerId);

	if (turnIt != m_turnOrder.end())
	{
		const auto removedIndex = static_cast<std::size_t>(
			std::distance(m_turnOrder.begin(), turnIt)
			);
		if (m_currentTurnIndex == removedIndex)
		{
			if (m_currentTurnIndex == m_turnOrder.size() - 1) m_currentTurnIndex = 0;
		}
		else if (m_currentTurnIndex > removedIndex) --m_currentTurnIndex;
		m_turnOrder.erase(turnIt);
	}

	MoveCardsToPile(it->second);
	m_players.erase(playerId);
	--m_playerCount;

	if (m_currentTurnIndex >= m_turnOrder.size() && !m_turnOrder.empty()) 
		m_currentTurnIndex = 0;
	return true;
}

void Table::MoveCardsToPile(Player& player)
{
	const auto& cards = player.GetCards();

	for (const auto& card : cards)
	{
		m_pile.push(card);
	}
}