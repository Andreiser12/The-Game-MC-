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
	explicit Player(unsigned playerID, std::string_view username);
	~Player() = default;

	Player(const Player&) = delete;
	Player& operator= (const Player&) = delete;
	Player(Player&&) noexcept = default;
	Player& operator= (Player&&) noexcept = default;

	void AddCard(Card<uint8_t>&& card);
	void DrawCards(Pile& deck);
	[[nodiscard]] bool FindCard(uint8_t cardId) const noexcept;
	void RemoveCard(uint8_t cardId) noexcept;

	void SetupCards(Pile& deck, uint8_t cardsCount);
	void IncrementCardsPlaced() noexcept;
	void ResetCardsPlaced() noexcept;

	[[nodiscard]] unsigned GetPlayerId() const noexcept;
	[[nodiscard]] const std::string& GetUsername() const noexcept;
	[[nodiscard]] const PlayerHand& GetCards() const noexcept;
	[[nodiscard]] size_t GetCardsCount() const noexcept;
	[[nodiscard]] uint8_t GetCardsPlacedRound() const noexcept;

	[[nodiscard]] bool operator<(const Player& other) const noexcept
	{
		return m_playerId < other.GetPlayerId();
	}

private:
	Player() = delete;

private:
	unsigned m_playerId;
	std::string m_username;
	PlayerHand m_cards;
	uint8_t m_cardsPlacedRound{ 0u };
};