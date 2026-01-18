export module Table;
import Card;
import Player;
import Deck;

import <unordered_map>;
import <algorithm>;
import <random>;
import <span>;
import <memory>;
import <array>; 
import <ranges>;
import <cstdint>;
import <optional>;

export class Table
{
public:
	explicit Table(std::span<const std::pair<unsigned, std::string_view>> lobbyPlayers);

	[[nodiscard]] bool IsPileEmpty() const noexcept;
	[[nodiscard]] bool BackwardsTrick(uint8_t deckId, const Card<uint8_t>& card) const;
	[[nodiscard]] bool PlayCard(unsigned playerId, uint8_t deckId, const Card<uint8_t>& card) const;
	void AdvanceTurn() noexcept;
	[[nodiscard]] bool HasPlayer(unsigned playerId) const noexcept;

	[[nodiscard]] bool CanPlayerPlayMinCards(const Player& player, uint8_t minCards) const;
	void PlaceCardOnDeck(uint8_t deckIndex, const Card<uint8_t>& card);
	[[nodiscard]] bool CanPlaceCard(unsigned playerId, uint8_t deckId, const Card<uint8_t>& card) const;
	[[nodiscard]] bool CheckWinCondition() const noexcept;
	[[nodiscard]] bool CheckLoseCondition() const;
	std::vector<uint8_t> EndTurn(unsigned playerId);
	bool RemovePlayer(unsigned playerId) noexcept;

	[[nodiscard]] const Deck& GetDeck(uint8_t deckId) const;
	[[nodiscard]] Deck& GetDeck(uint8_t deckId);
	[[nodiscard]] const Pile& GetPile() const noexcept { return m_pile; };
	[[nodiscard]] std::size_t GetDrawPileCount() const noexcept { return m_pile.size(); };
	std::span<const std::unique_ptr<Deck>> GetAllDecks() const noexcept;
	const std::unordered_map<unsigned, Player>& GetPlayers() const;
	[[nodiscard]] const Player& GetPlayer(unsigned playerId) const;
	[[nodiscard]] Player& GetPlayer(unsigned playerId);
	[[nodiscard]] unsigned GetCurrentPlayerId() const;

	std::optional<std::vector<std::pair<uint8_t, uint8_t>>> RunBotTurn(unsigned playerId,
		uint8_t cardsRequired);

private:
	void Shuffle();
	void MoveCardsToPile(Player& player);

private:
	std::unordered_map<unsigned, Player> m_players;
	std::vector<unsigned> m_turnOrder;
	Pile m_pile;
	std::array<std::unique_ptr<Deck>, 4> m_decks;

	uint8_t m_playerCount;	
	std::size_t m_currentTurnIndex{ 0 };
};

