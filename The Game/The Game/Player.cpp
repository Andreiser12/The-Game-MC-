module Player;

constexpr static uint8_t MINIMUM_CARDS{ 2u };

Player::Player(unsigned playerID, const std::string& name) :
	m_playerID{playerID}, m_username{name} {}

Player::Player(unsigned playerID, const std::unordered_set<Card, CardsSetHashFunction,
	CardsEqual>& cards, const UserProfile& userprofile) :
	m_playerID{ playerID }, m_cards { cards }, m_userprofile{ userprofile }  {}

Player::~Player() {}

void Player::PlaceCards(Deck& chosenDeck)
{
	uint8_t cardsPlaced = 0;
	while (GetCardsCount() || cardsPlaced < MINIMUM_CARDS)
	{
		std::cout << "Which card would you like to place?"
			<< std::endl;
		PrintCards();
		unsigned cardToPlace;

		std::cin >> cardToPlace;
		while (!FindCard(cardToPlace))
		{
			std::cout << "This card doesn't exist." <<
				std::endl << "Pleace choose an existing card: ";
			std::cin >> cardToPlace;
		}
		if (!IsCardValid(chosenDeck, cardToPlace))
		{
			std::cout << "You can't place this card on this deck!"
				<< std::endl;
			continue;
		}
		RemoveCard(cardToPlace);
		++cardsPlaced;
	}
	m_cardsPlacedRound = cardsPlaced;
}

void Player::PrintCards() const
{
	for (const Card& card : m_cards)
	{
		std::cout << card;
	}
	std::cout << std::endl;
}

void Player::DrawCards(std::stack<Card>& deck)
{
	while (!deck.empty() && m_cardsPlacedRound)
	{
		Card card = deck.top();
		deck.pop();
		m_cards.emplace(card);
		--m_cardsPlacedRound;
	}
}

bool Player::FindCard(uint16_t cardId) const
{
	Card cardToFind = Card(cardId);
	return m_cards.find(cardToFind) != m_cards.end();
}

void Player::RemoveCard(uint16_t cardId)
{
	Card cardToDelete(cardId);
	m_cards.erase(cardToDelete);
}

inline const bool Player::IsCardValid(const Deck& chosenDeck, uint16_t cardID)
{
	return (chosenDeck.GetDeckType() == Deck::DeckType::Ascending
		&& cardID > chosenDeck.GetTopCard().getId())
		|| (chosenDeck.GetDeckType() == Deck::DeckType::Descending
			&& cardID < chosenDeck.GetTopCard().getId());
}

void Player::SetupCards(std::stack<Card>& deck, uint8_t cardsCount)
{
	Card cardToPlace;
	for (uint8_t i = 0; i < cardsCount; i++)
	{
		cardToPlace = deck.top();
		deck.pop();
		m_cards.emplace(cardToPlace);
	}
}

void Player::SetCards(const std::unordered_set<Card, CardsSetHashFunction,
	CardsEqual>& cards)
{
	m_cards = cards;
}

unsigned Player::GetPlayerID() const
{
	return m_playerID;
}

const std::string& Player::GetUsername() const
{
	return m_username;
}

const std::unordered_set<Card, CardsSetHashFunction,
	CardsEqual>& Player::GetCards() const
{
	return m_cards;
}

size_t Player::GetCardsCount() const
{
	return m_cards.size();
}

UserProfile& Player::GetUserProfile()
{
	return m_userprofile;
}

bool Player::operator<(Player& other) const
{
	if (m_userprofile.PerformanceScore() == other.GetUserProfile().PerformanceScore())
	{
		return m_userprofile.GetUsername() < other.GetUserProfile().GetUsername();
	}
	return m_userprofile.PerformanceScore() > other.GetUserProfile().PerformanceScore();
}
