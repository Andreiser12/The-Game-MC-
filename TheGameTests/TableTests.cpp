#include "CppUnitTest.h"
#include <utility>
#include <string_view>

import Table;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheGameTests
{
	TEST_CLASS(TableTests)
	{
	public:
		TEST_METHOD(Table_Constructor_AddsPlayers)
		{
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{1u, "Alice"}, {2u, "Bob"}
			};
			Table table{ lobbyPlayers };
			Assert::IsTrue(table.HasPlayer(1u));
			Assert::IsTrue(table.HasPlayer(2u));
			Assert::AreEqual<size_t>(2, table.GetPlayers().size());
		}
		TEST_METHOD(Table_Constructor_InitialDeckCount)
		{
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{1u, "Mary"}, {2u, "John"}
			};
			Table table{ lobbyPlayers };
			auto decks = table.GetAllDecks();
			Assert::AreEqual<size_t>(4, decks.size());
		}
		TEST_METHOD(Table_BackwardsTrick_AscendingExactTen)
		{
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{1u, "Robert"}, {2u, "David"}
			};
			Table table{ lobbyPlayers };
			auto &deck = table.GetDeck(0);
			deck.PlaceCard(Card<uint8_t>{50});
			Assert::IsTrue(table.BackwardsTrick(0, Card<uint8_t>{40}));
		}
		TEST_METHOD(Table_BackwardsTrick_DescendingExactTen)
		{
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{1u, "James"}, {2u, "Liam"}
			};
			Table table{ lobbyPlayers };
			auto& deck = table.GetDeck(2);
			deck.PlaceCard(Card<uint8_t>{40});
			Assert::IsTrue(table.BackwardsTrick(2, Card<uint8_t>{50}));
		}
		TEST_METHOD(Table_BackwardsTrick_InvalidDifference)
		{
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{1u, "Noah"}, {2u, "Oliver"}
			};
			Table table{ lobbyPlayers };
			auto& deck = table.GetDeck(0);
			deck.PlaceCard(Card<uint8_t>{50});
			Assert::IsFalse(table.BackwardsTrick(0, Card<uint8_t>{45}));
		}
		TEST_METHOD(Table_PlayCard_AscendingAcceptsHigher)
		{
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{1u, "Michael"}, {2u, "Jennifer"}
			};
			Table table{ lobbyPlayers };
			unsigned playerId = table.GetCurrentPlayerId();
			auto& deck = table.GetDeck(0);
			deck.PlaceCard(Card<uint8_t>{30});
			Assert::IsTrue(table.PlayCard(playerId, 0, Card<uint8_t>{40}));
		}
		TEST_METHOD(Table_PlayCard_AscendingRejectsLower)
		{
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{1u, "Elizabeth"}, {2u, "Linda"}
			};
			Table table{ lobbyPlayers };
			unsigned playerId = table.GetCurrentPlayerId();
			auto& deck = table.GetDeck(0);
			deck.PlaceCard(Card<uint8_t>{30});
			Assert::IsFalse(table.PlayCard(playerId, 0, Card<uint8_t>{25}));
		}
		TEST_METHOD(Table_HasPlayer_Existing)
		{
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{1u, "Mary"}, {2u, "Emma"}
			};
			Table table{ lobbyPlayers };
			Assert::IsTrue(table.HasPlayer(1u));
			Assert::IsTrue(table.HasPlayer(2u));
		}
		TEST_METHOD(Table_HasPlayer_NonExisting)
		{
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{1u, "Olivia"}, {2u, "Amelia"}
			};
			Table table{ lobbyPlayers };
			Assert::IsFalse(table.HasPlayer(999u));
			Assert::IsFalse(table.HasPlayer(550u));
		}
	};
}