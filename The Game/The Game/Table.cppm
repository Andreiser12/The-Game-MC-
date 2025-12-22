export module Table;

import <iostream>;
import <vector>;
import <stack>;
import <unordered_set>;
import <unordered_map>;
import <algorithm>;
import <random>;
import <chrono>;
import <span>;
import <memory>;
import <array>;  

import Card;
import Player;
import Deck;
import DatabaseManager;

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
	void Shuffle();
	bool BackwardsTrick(uint8_t deckId, const Card<uint8_t>& card) const;
	bool CheckCard(uint8_t deckId, const Card<uint8_t>& card) const;
	void PlayGame();

	bool CanPlayerPlayMinCards(const Player& player, uint8_t minCards);
	void PlaceCardOnDeck(uint8_t deckIndex, const Card<uint8_t>& card);
	bool AllCardsPlaced() const;
	bool PlayerTurn(Player& player, uint8_t minCards);

	Deck& GetDeck(uint8_t deckId) const;
	const Pile& GetPile() const;
	std::span<std::reference_wrapper<Deck>> GetAllDecks();
	std::span<const Player> GetPlayers() const;
	const std::unordered_map<unsigned, Player>& GetPlayersMap() const;
	Player& GetPlayer(unsigned playerId);

	constexpr std::size_t GetDeckIndex(DeckIndex index) noexcept;

private:
	uint8_t m_nrPlayers;
	std::vector<Player> m_Players;
	std::unordered_map<unsigned, Player> m_playersMap;
	Pile m_pile;
	std::array<std::unique_ptr<Deck>, 4> m_decks;
};

