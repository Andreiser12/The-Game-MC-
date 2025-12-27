export module Table;

import <vector>;
import <stack>;
import <unordered_set>;
import <unordered_map>;
import <algorithm>;
import <random>;
import <span>;
import <memory>;
import <array>;  

#include "AssistantAI.h"

import Card;
import Player;
import Deck;
import DatabaseManager;

export enum class GameResult 
{
	Ongoing,
	Won,
	Lost,
};

export class Table
{
public:
	enum class DeckIndex : std::size_t 
	{
		FirstAscending = 0,
		SecondAscending = 1,
		FirstDescending = 2,
		SecondDescending = 3
	};

public:
	Table(const std::vector<std::pair<unsigned, std::string>>& lobbyPlayers);

	bool IsPileEmpty() const;
	bool BackwardsTrick(uint8_t deckId, const Card<uint8_t>& card) const;
	bool PlayCard(unsigned playerId, uint8_t deckId, const Card<uint8_t>& card) const;
	void AdvanceTurn();

	bool CanPlayerPlayMinCards(const Player& player, uint8_t minCards) const;
	void PlaceCardOnDeck(uint8_t deckIndex, const Card<uint8_t>& card);
	bool CheckWinCondition() const;
	bool CheckLoseCondition() const;
	std::vector<uint8_t> EndTurn(unsigned playerId);

	Deck& GetDeck(uint8_t deckId) const;
	const Pile& GetPile() const;
	std::span<std::reference_wrapper<Deck>> GetAllDecks() const;
	std::span<const Player> GetPlayers() const;
	const std::unordered_map<unsigned, Player>& GetPlayersMap() const;
	const Player& GetPlayer(unsigned playerId) const;
	Player& GetPlayer(unsigned playerId);
	unsigned GetCurrentPlayerId() const;
	std::size_t GetDrawPileCount() const;
	GameResult GetGameResult() const;

	constexpr std::size_t GetDeckIndex(DeckIndex index) const noexcept;

	void RunBotTurn(unsigned playerId);

private:
	void Shuffle();

private:
	uint8_t m_playerCount;
	std::vector<Player> m_Players;
	std::unordered_map<unsigned, Player> m_playersMap;
	Pile m_pile;
	std::array<std::unique_ptr<Deck>, 4> m_decks;
	std::vector<unsigned> m_turnOrder;
	std::size_t m_currentTurnIndex{ 0 };
	GameResult m_gameResult{ GameResult::Ongoing };
};

