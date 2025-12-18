module Table;

import GreedyAI;

constexpr static uint8_t  TWO_PLAYERS{ 2u };
constexpr static uint8_t  CARDS_TWO_PLAYERS{ 8u };

constexpr static uint8_t THREE_PLAYERS{ 3u };
constexpr static uint8_t  CARDS_THREE_PLAYERS{ 7u };

constexpr static uint8_t  CARDS_DEFAULT_PLAYERS{ 6u };

constexpr static uint8_t MIN_CARDS_NORMAL{ 2u };
constexpr static uint8_t MIN_CARDS_EMPTY_DECK{ 1u };

Table::Table(const std::vector<std::pair<unsigned, std::string>>& lobbyPlayers) :
	m_nrPlayers{ static_cast<uint8_t>(lobbyPlayers.size())}
{
	m_allDecks[0] = std::make_unique<Deck>(Deck::DeckType::Ascending, Deck::DeckNumber::First);
	m_allDecks[1] = std::make_unique<Deck>(Deck::DeckType::Ascending, Deck::DeckNumber::Second);
	m_allDecks[2] = std::make_unique<Deck>(Deck::DeckType::Descending, Deck::DeckNumber::First);
	m_allDecks[3] = std::make_unique<Deck>(Deck::DeckType::Descending, Deck::DeckNumber::Second);

	Shuffle();

	size_t cardsPerPlayer;
	switch (m_nrPlayers)
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
		player.SetupCards(m_mainDeck, cardsPerPlayer);
		m_playersMap.emplace(id, std::move(player));
	}

	//std::sort(m_Players.begin(), m_Players.end());
}

bool Table::isDeckEmpty() const
{
	return m_mainDeck.empty();
}

void Table::Shuffle()
{
	std::vector<Card<uint16_t>> copyCards;
	copyCards.reserve(98);

	for (uint16_t i = 2u; i < 100u; i++)
		copyCards.emplace_back(i);

	std::random_device rd;
	std::mt19937 generator(rd());
	std::shuffle(copyCards.begin(), copyCards.end(), generator);
	
	for (auto& card : copyCards)
		m_mainDeck.push(std::move(card));
}

bool Table::BackwardsTrick(const Deck& deck, const Card<uint16_t>& card) const
{
	const unsigned cardValue = card.getId();
	const unsigned topCardValue = deck.GetTopCard().getId();

	switch (deck.GetDeckType())
	{
		case Deck::DeckType::Ascending:
			return (topCardValue>=10u && topCardValue - 10u == cardValue);
		case Deck::DeckType::Descending:
			return (topCardValue + 10u == cardValue);
		default:
			return false;
	}
}

bool Table::CheckCard(const Deck& deck, const Card<uint16_t>& card) const
{
	const unsigned cardValue = card.getId();
	const unsigned topCardValue = deck.GetTopCard().getId();

	switch (deck.GetDeckType())
	{
	case Deck::DeckType::Ascending:
		return cardValue > topCardValue || BackwardsTrick(deck, card);
	case Deck::DeckType::Descending:
		return cardValue < topCardValue || BackwardsTrick(deck, card);
	default:
		return false;
	}
}

constexpr std::size_t Table::GetDeckIndex(DeckIndex index) noexcept
{
	return static_cast<std::size_t>(index);
}

Deck& Table::GetDeck(const Deck::DeckType& type, const Deck::DeckNumber& number)
{
	if (type == Deck::DeckType::Ascending)
	{
		if (number == Deck::DeckNumber::First)
			return *m_allDecks[GetDeckIndex(DeckIndex::FirstAscending)];
		else if (number == Deck::DeckNumber::Second)
			return *m_allDecks[GetDeckIndex(DeckIndex::SecondAscending)];
	}
	else if (type == Deck::DeckType::Descending)
	{
		if (number == Deck::DeckNumber::First)
			return *m_allDecks[GetDeckIndex(DeckIndex::FirstDescending)];
		else if (number == Deck::DeckNumber::Second)
			return *m_allDecks[GetDeckIndex(DeckIndex::SecondDescending)];
	}
	throw std::out_of_range("Invalid deck type or number.");
}

std::span<std::reference_wrapper<Deck>> Table::GetAllDecks()
{
	static std::array<std::reference_wrapper<Deck>, 4> decks{
		*m_allDecks[0],
		*m_allDecks[1],
		*m_allDecks[2],
		*m_allDecks[3]
	};

	return {decks};
}


const DeckStack& Table::GetMainDeck() const
{
	return m_mainDeck;
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

		uint8_t minCards = isDeckEmpty() ? MIN_CARDS_EMPTY_DECK : MIN_CARDS_NORMAL;

		if (!CanPlayerPlayMinCards(currentPlayer, minCards))
		{
			gameWon = false;
			gameRunning = false;
			break;
		}
		bool turnSucces = PlayerTurn(currentPlayer, minCards);

		if (!turnSucces)
		{
			gameWon = false;
			gameRunning = false;
			break;
		}
		currentPlayerIndex = (currentPlayerIndex + 1) % m_nrPlayers;
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

bool Table::CanPlayerPlayMinCards(const Player& player, uint8_t minCards) 
{
	if (player.GetCardsCount() < minCards)
		return false;
	uint8_t validCardsCount = 0;
	for (const Card<uint16_t>& card : player.GetCards())
	{
		for (auto& deck : GetAllDecks())
		{
			if (CheckCard(deck, card))
			{
				validCardsCount++;
				if (validCardsCount >= minCards)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void Table::PlaceCardOnDeck(Deck& deck, const Card<uint16_t>& card)
{
	deck.PushCard(card);
}

bool Table::AllCardsPlaced() const
{
	if (!isDeckEmpty())
		return false;

	for (const Player& player : m_Players) {
		if (player.GetCardsCount() > 0)
			return false;
	}
	return true;
}

bool Table::PlayerTurn(Player& player, uint8_t minCards)
{

	if (player.HasTimeOut() || player.IsInactive())
	{
		GreedyAI::PlayTurn(player, *this, minCards);
		return true;
	}

	uint8_t cardsPlaced = 0u;
	bool continueTurn = true;

	while (continueTurn)
	{
		player.PrintCards();

		if (cardsPlaced >= minCards)
		{
			unsigned choice;
			std::cin >> choice;
			if (choice == 0u)
			{
				continueTurn = false;
				break;
			}
		}

		uint16_t cardId;
		std::cin >> cardId;

		if (!player.FindCard(cardId))
		{
			continue;
		}

		unsigned deckChoice;
		std::cin >> deckChoice;

		Deck& chosenDeck = [&]() ->Deck& {
			switch (deckChoice)
			{
			case 1u:
				return GetDeck(Deck::DeckType::Ascending, Deck::DeckNumber::First);
				break;
			case 2u:
				return GetDeck(Deck::DeckType::Ascending, Deck::DeckNumber::Second);
				break;
			case 3u:
				return GetDeck(Deck::DeckType::Descending, Deck::DeckNumber::First);
				break;
			case 4u:
				return GetDeck(Deck::DeckType::Descending, Deck::DeckNumber::Second);
				break;
			default:
				throw std::invalid_argument("Invalid deck choice");
			}}();
	
		Card cardToPlace(cardId);
		if (!CheckCard(chosenDeck, cardToPlace)) {
			continue;
		}

		PlaceCardOnDeck(chosenDeck, cardToPlace);
		player.RemoveCard(cardId);
		cardsPlaced++;

		if (player.GetCardsCount() == 0)
		{
			continueTurn = false;
			break;
		}
	}
	if (!isDeckEmpty() && cardsPlaced > 0)
	{
		uint8_t cardsToDraw = std::min(static_cast<uint8_t>(cardsPlaced), static_cast<uint8_t>(m_mainDeck.size()));
		player.DrawCards(m_mainDeck);
	}
	return cardsPlaced >= minCards;
}
