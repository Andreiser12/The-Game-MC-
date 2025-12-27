module Table;

import GreedyAI;

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

Table::Table(const std::vector<std::pair<unsigned, std::string>>& lobbyPlayers) :
	m_playerCount{ static_cast<uint8_t>(lobbyPlayers.size())}
{
	m_decks[0] = std::make_unique<Deck>(Deck::DeckType::Ascending, Deck::DeckNumber::First, 0u);
	m_decks[1] = std::make_unique<Deck>(Deck::DeckType::Ascending, Deck::DeckNumber::Second, 1u);
	m_decks[2] = std::make_unique<Deck>(Deck::DeckType::Descending, Deck::DeckNumber::First, 2u);
	m_decks[3] = std::make_unique<Deck>(Deck::DeckType::Descending, Deck::DeckNumber::Second, 3u);
	Shuffle();

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
		Player player(id, username);
		player.SetupCards(m_pile, cardsPerPlayer);
		m_playersMap.emplace(id, std::move(player));
		m_turnOrder.emplace_back(id);
	}
}

bool Table::IsPileEmpty() const
{
	return m_pile.empty();
}

void Table::Shuffle()
{
	std::vector<Card<uint8_t>> copyCards;
	copyCards.reserve(98);

	for (uint8_t i = 2u; i < 100u; i++)
		copyCards.emplace_back(i);

	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(copyCards.begin(), copyCards.end(), generator);
	
	for (auto& card : copyCards)
	{
		m_pile.push(std::move(card));
	}	
}

bool Table::BackwardsTrick(uint8_t deckId, const Card<uint8_t>& card) const
{
	const uint8_t cardValue = card.getId();
	const uint8_t topCardValue = m_decks[deckId]->GetTopCard().getId();

	switch (m_decks[deckId]->GetDeckType())
	{
		case Deck::DeckType::Ascending:
			return (topCardValue >= 10u && topCardValue - 10u == cardValue);
		case Deck::DeckType::Descending:
			return (topCardValue + 10u == cardValue);
		default:
			return false;
	}
}

bool Table::PlayCard(unsigned playerId, uint8_t deckId, const Card<uint8_t>& card) const
{
	if (playerId != GetCurrentPlayerId()) return false;
	
	const uint8_t cardValue = card.getId();
	const uint8_t topCardValue = m_decks[deckId]->GetTopCard().getId();

	switch (m_decks[deckId]->GetDeckType())
	{
	case Deck::DeckType::Ascending:
		return cardValue > topCardValue || BackwardsTrick(deckId, card);
	case Deck::DeckType::Descending:
		return cardValue < topCardValue || BackwardsTrick(deckId, card);
	default:
		return false;
	}
}

void Table::AdvanceTurn()
{
	Player& currentPlayer = GetPlayer(GetCurrentPlayerId());

	if (currentPlayer.HasTimeOut())
	{
		RunBotTurn(currentPlayer.GetPlayerId());
	}

	m_currentTurnIndex = (m_currentTurnIndex + 1) % m_playerCount;
}

constexpr std::size_t Table::GetDeckIndex(DeckIndex index) const noexcept
{
	return static_cast<std::size_t>(index);
}

void Table::RunBotTurn(unsigned playerId)
{
	Player& player = GetPlayer(playerId);

	uint8_t minCardsToPlay = IsPileEmpty() ? MIN_CARDS_EMPTY_DECK : MIN_CARDS_NORMAL;
	int cardsPlayed = 0;
	bool canPlayMore = true;

	while (canPlayMore)
	{
		std::vector<uint8_t> myHand;
		for (const auto& card : player.GetCards())
		{
			myHand.push_back(card.getId());
		}

		std::vector<AssistantAI::DeckState> deckStates;
		auto allDecks = GetAllDecks();

		for (const auto& deckRef : allDecks) {
			Deck& d = deckRef.get();
			AssistantAI::DeckState ds;
			ds.index = static_cast<int>(d.GetDeckIndex());
			ds.topCard = d.GetTopCard().getId();
			ds.type = ConvertDeckType(d.GetDeckType());
			deckStates.push_back(ds);
		}

		std::vector<std::vector<uint8_t>> otherHands;
		for (const auto& [id, otherPlayer] : m_playersMap)
		{
			if (id != playerId) 
			{
				std::vector<uint8_t> hand;
				for (const auto& c : otherPlayer.GetCards()) 
				{
					hand.push_back(c.getId());
				}
				otherHands.push_back(hand);
			}
		}

		auto resultOpt = AssistantAI::GreedyBot::FindBestMove(myHand, deckStates, otherHands);

		if (!resultOpt.has_value()) 
		{
			canPlayMore = false;
		}
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

				cardsPlayed++;
			}
		}

		if (player.GetCardsCount() == 0) canPlayMore = false;
	}

	EndTurn(playerId);
}

std::size_t Table::GetDrawPileCount() const
{
	return m_pile.size();
}

GameResult Table::GetGameResult() const
{
	return m_gameResult;
}

Deck& Table::GetDeck(uint8_t deckId) const
{
	return *m_decks[deckId];
}

std::span<std::reference_wrapper<Deck>> Table::GetAllDecks() const
{
	static std::array<std::reference_wrapper<Deck>, 4> decks{
		*m_decks[0],
		*m_decks[1],
		*m_decks[2],
		*m_decks[3]
	};
	return {decks};
}

const Pile& Table::GetPile() const
{
	return m_pile;
}

std::span<const Player> Table::GetPlayers() const
{
	return {m_Players.data(), m_Players.size()};
}

const std::unordered_map<unsigned, Player>& Table::GetPlayersMap() const
{
	return m_playersMap;
}

const Player& Table::GetPlayer(unsigned playerId) const
{
	auto it = m_playersMap.find(playerId);
	if (it == m_playersMap.end())
		throw std::runtime_error("Player not found");
	return it->second;
}

Player& Table::GetPlayer(unsigned playerId) 
{
	return const_cast<Player&>(
		static_cast<const Table&>(*this).GetPlayer(playerId));
}

unsigned Table::GetCurrentPlayerId() const
{
	return m_turnOrder[m_currentTurnIndex];
}

bool Table::CanPlayerPlayMinCards(const Player& player, uint8_t minCards) const
{
	if (static_cast<uint8_t>(player.GetCardsCount()) < minCards) return false;
	uint8_t validCards{0u};

	for (const Card<uint8_t>& card : player.GetCards())
	{
		for (size_t deckIndex = 0; deckIndex<m_decks.size(); deckIndex++)
		{
			if (PlayCard(player.GetPlayerId(), deckIndex, card))
			{
				validCards++;
				if (validCards >= minCards) return true;
			}
		}
	}
	return false;
}

void Table::PlaceCardOnDeck(uint8_t deckIndex, const Card<uint8_t>& card)
{
	m_decks[deckIndex]->PushCard(card);
}

bool Table::CheckWinCondition() const
{
	if (!IsPileEmpty()) return false;

	for (const auto& [id, player] : m_playersMap)
	{
		if (player.GetCardsCount()) return false;
	}
	return true;
}

bool Table::CheckLoseCondition() const
{
	const Player& currentPlayer = GetPlayer(GetCurrentPlayerId());
	uint8_t minCards = m_pile.empty() ? 1u : 2u;
	return !CanPlayerPlayMinCards(currentPlayer, minCards);
}

std::vector<uint8_t> Table::EndTurn(unsigned playerId)
{
	if (playerId != GetCurrentPlayerId()) return {};

	Player& player = GetPlayer(playerId);	
	const uint8_t cardsPlaced = player.GetCardsPlacedRound();
	uint8_t drawCount = static_cast<uint8_t>(std::min<std::size_t>(cardsPlaced, m_pile.size()));
	std::vector<uint8_t> drawnCards;
	for (uint8_t i = 0; i < drawCount; i++)
	{
		Card<uint8_t> card = m_pile.top();
		m_pile.pop();
		player.AddCard(card);
		drawnCards.emplace_back(card.getId());
	}
	player.ResetCardsPlaced();
	AdvanceTurn();
	if (CheckWinCondition())
	{
		m_gameResult = GameResult::Won;
	}
	if (CheckLoseCondition())
	{
		m_gameResult = GameResult::Lost;
	}
	return drawnCards;
}
