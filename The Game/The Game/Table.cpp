module Table;

import GreedyAI;

constexpr static uint8_t TWO_PLAYERS{ 2u };
constexpr static uint8_t CARDS_TWO_PLAYERS{ 8u };

constexpr static uint8_t THREE_PLAYERS{ 3u };
constexpr static uint8_t CARDS_THREE_PLAYERS{ 7u };

constexpr static uint8_t CARDS_DEFAULT_PLAYERS{ 6u };

constexpr static uint8_t MIN_CARDS_NORMAL{ 2u };
constexpr static uint8_t MIN_CARDS_EMPTY_DECK{ 1u };

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
	}

	//std::sort(m_Players.begin(), m_Players.end());
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

bool Table::CheckCard(uint8_t deckId, const Card<uint8_t>& card) const
{
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

constexpr std::size_t Table::GetDeckIndex(DeckIndex index) noexcept
{
	return static_cast<std::size_t>(index);
}

std::size_t Table::GetDrawPileCount() const
{
	return m_pile.size();
}

Deck& Table::GetDeck(uint8_t deckId) const
{
	return *m_decks[deckId];
}

std::span<std::reference_wrapper<Deck>> Table::GetAllDecks()
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

void Table::PlayGame()
{
	bool gameRunning = true;
	bool gameWon = false;
	uint8_t currentPlayerIndex = 0u;
	auto startTime = std::chrono::high_resolution_clock::now();

	while (gameRunning)
	{
		if (AllCardsPlaced())
		{
			gameWon = true;
			gameRunning = false;
			break;
		}
		Player& currentPlayer = m_Players[currentPlayerIndex];

		uint8_t minCards = IsPileEmpty() ? MIN_CARDS_EMPTY_DECK : MIN_CARDS_NORMAL;

		if (!CanPlayerPlayMinCards(currentPlayer, minCards))
		{
			gameWon = false;
			gameRunning = false;
			break;
		}

		currentPlayerIndex = (currentPlayerIndex + 1) % m_playerCount;
	}

	for (Player& player : m_Players)
	{
		size_t finalCards = player.GetCardsCount();
	}
}

std::span<const Player> Table::GetPlayers() const
{
	return {m_Players.data(), m_Players.size()};
}

const std::unordered_map<unsigned, Player>& Table::GetPlayersMap() const
{
	return m_playersMap;
}

Player& Table::GetPlayer(unsigned playerId)
{
	auto it = m_playersMap.find(playerId);
	if (it == m_playersMap.end())
	{
		throw std::runtime_error("Player not found");
	}
	return it->second;
}

bool Table::CanPlayerPlayMinCards(const Player& player, uint8_t minCards) 
{
	if (player.GetCardsCount() < minCards) return false;
	uint8_t validCardsCount{0};

	for (const Card<uint8_t>& card : player.GetCards())
	{
		for (size_t deckIndex = 0; deckIndex<4; deckIndex++)
		{
			if (CheckCard(deckIndex, card))
			{
				validCardsCount++;
				if (validCardsCount >= minCards) return true;
			}
		}
	}
	return false;
}

void Table::PlaceCardOnDeck(uint8_t deckIndex, const Card<uint8_t>& card)
{
	m_decks[deckIndex]->PushCard(card);
}

bool Table::AllCardsPlaced() const
{
	if (!IsPileEmpty()) return false;

	for (const auto& [id, player] : m_playersMap)
	{
		if (player.GetCardsCount()) return false;
	}
	return true;
}

std::vector<uint8_t> Table::EndTurn(unsigned playerId)
{
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
	return drawnCards;
}
