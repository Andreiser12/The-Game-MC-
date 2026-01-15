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

#include "AssistantAI.h"
export class Table
{
public:
	explicit Table(std::span<const std::pair<unsigned, std::string_view>> lobbyPlayers);

	bool IsPileEmpty() const noexcept;
	bool BackwardsTrick(uint8_t deckId, const Card<uint8_t>& card) const;
	bool PlayCard(unsigned playerId, uint8_t deckId, const Card<uint8_t>& card) const;
	void AdvanceTurn() noexcept;
	bool HasPlayer(unsigned playerId) const;

	bool CanPlayerPlayMinCards(const Player& player, uint8_t minCards) const;
	void PlaceCardOnDeck(uint8_t deckIndex, const Card<uint8_t>& card);
	bool CheckWinCondition() const noexcept;
	bool CheckLoseCondition() const;
	std::vector<uint8_t> EndTurn(unsigned playerId);
	bool RemovePlayer(unsigned playerId) noexcept;

	Deck& GetDeck(uint8_t deckId) const;
	[[nodiscard]] const Pile& GetPile() const noexcept { return m_pile; };
	[[nodiscard]] std::size_t GetDrawPileCount() const noexcept { return m_pile.size(); };
	std::span<const std::unique_ptr<Deck>> GetAllDecks() const noexcept;
	const std::unordered_map<unsigned, Player>& GetPlayers() const;
	const Player& GetPlayer(unsigned playerId) const;
	Player& GetPlayer(unsigned playerId);
	unsigned GetCurrentPlayerId() const;

	void RunBotTurn(unsigned playerId);

private:
	void Shuffle();
	void MoveCardsToPile(Player& player);
	bool CanPlayOnDeck(uint8_t cardValue, uint8_t topValue, size_t deckIndex) const;

private:
	uint8_t m_playerCount;
	std::unordered_map<unsigned, Player> m_players;
	Pile m_pile;
	std::array<std::unique_ptr<Deck>, 4> m_decks;
	std::vector<unsigned> m_turnOrder;
	std::size_t m_currentTurnIndex{ 0 };
};

