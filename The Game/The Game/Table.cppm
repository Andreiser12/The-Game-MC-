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
//import GreedyAI;

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

	bool isDeckEmpty() const;
	void Shuffle();
	bool BackwardsTrick(const Deck& deck, const Card<uint16_t>& card) const;
	bool CheckCard(const Deck& deck, const Card<uint16_t>& card) const;
	void PlayGame();

	bool CanPlayerPlayMinCards(const Player& player, uint8_t minCards);
	void PlaceCardOnDeck(Deck& deck, const Card<uint16_t>& card);
	bool AllCardsPlaced() const;
	bool PlayerTurn(Player& player, uint8_t minCards);

	Deck& GetDeck(const Deck::DeckType &type, const Deck::DeckNumber &number);
	const DeckStack& GetMainDeck() const;
	std::span<std::reference_wrapper<Deck>> GetAllDecks();
	std::span<const Player> GetPlayers() const;
	const std::unordered_map<unsigned, Player>& GetPlayersMap() const;

	constexpr std::size_t GetDeckIndex(DeckIndex index) noexcept;

private:
	uint8_t m_nrPlayers;
	std::vector<Player> m_Players;
	std::unordered_map<unsigned, Player> m_playersMap;
	DeckStack m_mainDeck;
	std::array<std::unique_ptr<Deck>, 4> m_allDecks;
};

