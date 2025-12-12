export module Table;

import <iostream>;
import <vector>;
import <stack>;
import <unordered_set>;
import <algorithm>;
import <random>;
import <chrono>;
import <span>;

import Card;
import Player;
import UserProfile;
import Deck;
import DatabaseManager;

export class Table
{
public:
	Table(uint8_t nrPlayers);

	bool isDeckEmpty() const;
	void Shuffle();
	void SetupPlayers();
	bool BackwardsTrick(const Deck& deck, const Card<uint16_t>& card) const;
	bool CheckCard(const Deck& deck, const Card<uint16_t>& card) const;
	void PlayGame();

	void DisplayTableState() const;
	bool CanPlayerPlayMinCards(const Player& player, uint8_t minCards);
	void PlaceCardOnDeck(Deck& deck, const Card<uint16_t>& card);
	bool AllCardsPlaced() const;
	bool PlayerTurn(Player& player, uint8_t minCards);

	Deck& GetDeck(const Deck::DeckType &type, const Deck::DeckNumber &number);
	const DeckStack& GetMainDeck() const;
	std::span<Deck*> GetAllDecks();

private:
	uint8_t m_nrPlayers;
	std::vector<Player> m_Players;
	DeckStack m_mainDeck;
	Deck m_AscendingDeckFirst;
	Deck m_AscendingDeckSecond;
	Deck m_DescendingDeckFirst;
	Deck m_DescendingDeckSecond;

	unsigned int m_time;
};

