module Table;

import AssistantAI;

constexpr static size_t DECK_SIZE{ 98 };
constexpr static uint8_t DECK_COUNT{ 4u };

constexpr static uint8_t MIN_CARDS_NORMAL{ 2u };
constexpr static uint8_t MIN_CARDS_EMPTY_DECK{ 1u };

static AssistantAI::DeckType ConvertDeckType(Deck::DeckType type)
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
			it->second.SetupCards(m_pile, static_cast<uint8_t>(cardsPerPlayer));
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
	cards.reserve(DECK_SIZE);

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
	m_currentTurnIndex = (m_currentTurnIndex + 1) % m_turnOrder.size();
}

bool Table::HasPlayer(unsigned playerId) const noexcept
{
	return m_players.contains(playerId);
}

std::optional<std::vector<std::pair<uint8_t, uint8_t>>> Table::RunBotTurn
(unsigned playerId, uint8_t cardsRequired)
{
	Player& player{ GetPlayer(playerId) };

	std::vector<uint8_t> myHand;
	for (const auto& card : player.GetCards())
	{
		myHand.emplace_back(card.GetId());
	}

	std::vector<AssistantAI::DeckState> deckStates;
	auto allDecks = GetAllDecks();
	for (const auto& deckRef : allDecks)
	{
		AssistantAI::DeckState state;
		state.id = static_cast<int>(deckRef->GetDeckIndex());
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
				hand.emplace_back(card.GetId());
			}
			otherHands.emplace_back(hand);
		}
	}
	
	std::vector<std::pair<uint8_t, uint8_t>> selectedCards;
	selectedCards.reserve(cardsRequired);

	std::vector<uint8_t> tempHand = myHand;
	std::vector<AssistantAI::DeckState> tempDecks = deckStates;

	for (uint8_t i = 0u; i < cardsRequired; ++i)
	{
		auto moveOpt = AssistantAI::GreedyBot::FindBestMove(tempHand, tempDecks, otherHands);
		if (!moveOpt) return std::nullopt;

		const auto& move = moveOpt.value();
		selectedCards.emplace_back(move.cardValue, static_cast<uint8_t>(move.deckId));
		tempHand.erase(std::find(tempHand.begin(), tempHand.end(), move.cardValue));

		for (auto& deck : tempDecks)
		{
			if (deck.id == move.deckId)
			{
				deck.topCard = move.cardValue;
				break;
			}
		}

		if (tempHand.empty()) break;
	}

	if (selectedCards.size() < cardsRequired) return std::nullopt;
	return selectedCards;
}

const Deck& Table::GetDeck(uint8_t deckId) const
{
	return *m_decks[deckId];
}

Deck& Table::GetDeck(uint8_t deckId)
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
	const uint8_t cardCount{ static_cast<uint8_t>(player.GetCardsCount()) };
	if (cardCount == 0) return true;
	if (cardCount < minCards) return false;

	std::array<uint8_t, DECK_COUNT> deckTops;
	for (size_t i = 0; i < DECK_COUNT; ++i)
	{
		deckTops[i] = m_decks[i]->GetTopCard().GetId();
	}

	auto cards = player.GetCards();
	uint8_t validCards{ 0u };

	while (validCards < minCards && !cards.empty())
	{
		bool placedCard{ false };
		for (auto it = cards.begin(); it != cards.end(); ++it)
		{
			const uint8_t cardValue{ it->GetId() };
			for (size_t deckIndex = 0; deckIndex < DECK_COUNT; ++deckIndex)
			{
				if (CanPlaceCard(player.GetPlayerId(), static_cast<uint8_t>(deckIndex), *it))
				{
					deckTops[deckIndex] = cardValue;
					cards.erase(it);
					++validCards;
					placedCard = true;
					break;
				}
			}
			if (placedCard) break;
		}
		if (!placedCard) break;
	}
	return validCards >= minCards;
}

void Table::PlaceCardOnDeck(uint8_t deckIndex, const Card<uint8_t>& card)
{
	m_decks[deckIndex]->PlaceCard(card);
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
		player.AddCard(std::move(m_pile.top()));
		m_pile.pop();
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

bool Table::CanPlaceCard(unsigned playerId, uint8_t deckId, const Card<uint8_t>& card) const
{
	if (playerId != GetCurrentPlayerId()) return false;

	const uint8_t cardValue{ card.GetId() };
	const uint8_t topValue{ m_decks[deckId]->GetTopCard().GetId() };
	const bool isBackwards{ BackwardsTrick(deckId, card) };

	if (m_decks[deckId]->GetDeckType() == Deck::DeckType::Ascending)
	{
		return cardValue > topValue || isBackwards;
	}
	return cardValue < topValue || isBackwards;
}
