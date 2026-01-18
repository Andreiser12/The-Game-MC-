module Player;

Player::Player(unsigned playerID, std::string_view username) :
	m_playerId{ playerID }, m_username{ username } {
}

void Player::AddCard(Card<uint8_t>&& card)
{
	m_cards.emplace(std::move(card));
}

void Player::DrawCards(Pile& deck)
{
	while (!deck.empty() && m_cardsPlacedRound)
	{
		m_cards.emplace(std::move(deck.top()));
		deck.pop();
		--m_cardsPlacedRound;
	}
}

bool Player::FindCard(uint8_t cardId) const noexcept
{
	return m_cards.contains(Card<uint8_t>{cardId});
}

void Player::RemoveCard(uint8_t cardId) noexcept
{
	auto it{ std::find_if(m_cards.begin(), m_cards.end(),
		[cardId](const Card<uint8_t>& card)
		{
			return card.GetId() == cardId;
		}) };
	if (it != m_cards.end()) m_cards.erase(it);
}

void Player::SetupCards(Pile& deck, uint8_t cardsCount)
{
	m_cards.reserve(cardsCount);
	for (uint8_t i = 0; i < cardsCount; ++i)
	{
		m_cards.emplace(std::move(deck.top()));
		deck.pop();
	}
}

void Player::IncrementCardsPlaced() noexcept
{
	++m_cardsPlacedRound;
}

void Player::ResetCardsPlaced() noexcept
{
	m_cardsPlacedRound = 0u;
}

unsigned Player::GetPlayerId() const noexcept
{
	return m_playerId;
}

const std::string& Player::GetUsername() const noexcept
{
	return m_username;
}

const PlayerHand& Player::GetCards() const noexcept
{
	return m_cards;
}

size_t Player::GetCardsCount() const noexcept
{
	return m_cards.size();
}

uint8_t Player::GetCardsPlacedRound() const noexcept
{
	return m_cardsPlacedRound;
}