module Player;

constexpr static uint8_t MINIMUM_CARDS{ 2u };

Player::Player(unsigned playerID, const std::string& username) :
	m_playerId{playerID}, m_username{username} {}

Player::~Player() {}

void Player::PlaceCards(Deck& chosenDeck)
{
	uint8_t cardsPlaced = 0u;
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
	for (const Card<uint8_t>& card : m_cards)
	{
		std::cout << card;
	}
	std::cout << std::endl;
}

void Player::DrawCards(Pile& deck)
{
	while (!deck.empty() && m_cardsPlacedRound)
	{
		Card<uint8_t> card = deck.top();
		deck.pop();
		m_cards.emplace(card);
		--m_cardsPlacedRound;
	}
}

bool Player::FindCard(uint8_t cardId) const
{
	Card<uint8_t> cardToFind = Card(cardId);
	return m_cards.find(cardToFind) != m_cards.end();
}

void Player::RemoveCard(uint8_t cardId)
{
	auto it = std::find_if(m_cards.begin(), m_cards.end(),
		[cardId](const Card<uint8_t>& card)
		{
			return card.getId() == cardId;
		});
	if (it != m_cards.end()) m_cards.erase(it);
}

inline const bool Player::IsCardValid(const Deck& chosenDeck, uint8_t cardId)
{
	return (chosenDeck.GetDeckType() == Deck::DeckType::Ascending
		&& cardId > chosenDeck.GetTopCard().getId())
		|| (chosenDeck.GetDeckType() == Deck::DeckType::Descending
			&& cardId < chosenDeck.GetTopCard().getId());
}

void Player::SetupCards(Pile& deck, uint8_t cardsCount)
{
	Card<uint8_t> cardToPlace;
	for (uint8_t i = 0; i < cardsCount; i++)
	{
		cardToPlace = deck.top();
		deck.pop();
		m_cards.emplace(cardToPlace);
	}
}

unsigned Player::GetPlayerId() const
{
	return m_playerId;
}

const std::string& Player::GetUsername() const
{
	return m_username;
}

const PlayerHand& Player::GetCards() const
{
	return m_cards;
}

size_t Player::GetCardsCount() const
{
	return m_cards.size();
}

bool Player::operator<(Player& other) const
{
	return this->m_playerId < other.GetPlayerId();
}

void Player::SetTimeOut(bool value)
{
	m_hasTimeOut = value;
}
void Player::SetIncative(bool value)
{
	m_isInactive = value;
}

bool Player::HasTimeOut() const
{
	return m_hasTimeOut;
}

bool Player::IsInactive() const
{
	return m_isInactive;
}