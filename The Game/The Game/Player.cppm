export module Player;

import Card;
import Deck;

import <unordered_set>;
import <cstddef>;
import <string>;
import <functional>;

export using PlayerHand = std::unordered_set<Card<std::uint8_t>>;

export class Player
{
public:
	Player(unsigned playerID, std::string_view username);
	~Player() = default;

	void AddCard(Card<uint8_t>&& card);
	void DrawCards(Pile& deck);
	[[nodiscard]] bool FindCard(uint8_t cardId) const noexcept;
	void RemoveCard(uint8_t cardId) noexcept;

	void SetupCards(Pile& deck, uint8_t cardsCount);
	void IncrementCardsPlaced() noexcept;
	void ResetCardsPlaced() noexcept;

	void SetTimeOut(bool value);
	void SetIncative(bool value);

	unsigned GetPlayerId() const noexcept;
	const std::string& GetUsername() const noexcept;
	const PlayerHand& GetCards() const noexcept;
	size_t GetCardsCount() const noexcept;
	uint8_t GetCardsPlacedRound() const noexcept;

	[[nodiscard]] bool operator<(const Player& other) const noexcept
	{
		return m_playerId < other.GetPlayerId();
	}

	bool HasTimeOut() const noexcept;
	bool IsInactive() const noexcept;

private:
	Player() = delete;

private:
	unsigned m_playerId;
	std::string m_username;
	PlayerHand m_cards;
	uint8_t m_cardsPlacedRound{ 0u };

	bool m_hasTimeOut{ false };
	bool m_isInactive{ false };
};

