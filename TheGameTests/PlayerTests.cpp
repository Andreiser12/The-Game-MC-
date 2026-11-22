#include "CppUnitTest.h"
import Player;
import Deck;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheGameTests
{
	TEST_CLASS(PlayerTests)
	{
	public:
		TEST_METHOD(Player_Constructor_InitialState)
		{
			Player currentPlayer{ 1u, "Alice" };
			Assert::AreEqual(1u, currentPlayer.GetPlayerId());
			Assert::AreEqual(std::string("Alice"), currentPlayer.GetUsername());
			Assert::AreEqual<size_t>(0, currentPlayer.GetCardsCount());
		}
		TEST_METHOD(Player_AddCard_FindCard)
		{
			Player currentPlayer{ 2u, "Bob" };
			currentPlayer.AddCard(Card<uint8_t>{10});
			Assert::IsTrue(currentPlayer.FindCard(10));
			Assert::AreEqual<size_t>(1, currentPlayer.GetCardsCount());
		}
		TEST_METHOD(Player_RemoveCard_RemovesExisting)
		{
			Player currentPlayer{ 3u, "Carol" };
			currentPlayer.AddCard(Card<uint8_t>{20});
			currentPlayer.RemoveCard(20);
			Assert::IsFalse(currentPlayer.FindCard(20));
			Assert::AreEqual<size_t>(0, currentPlayer.GetCardsCount());
		}
		TEST_METHOD(Player_RemoveCard_NonExisting_NoEffect)
		{
			Player currentPlayer{ 4u, "Dan" };
			currentPlayer.AddCard(Card<uint8_t>{30});
			currentPlayer.RemoveCard(99);
			Assert::AreEqual<size_t>(1, currentPlayer.GetCardsCount());
		}
		TEST_METHOD(Player_SetupCards_DrawsExactCount)
		{
			Pile deck;
			deck.push(Card<uint8_t>{1});
			deck.push(Card<uint8_t>{2});
			deck.push(Card<uint8_t>{3});
			Player currentPlayer{ 5u, "Eve" };
			currentPlayer.SetupCards(deck, 2);
			Assert::AreEqual<size_t>(2, currentPlayer.GetCardsCount());
			Assert::AreEqual<size_t>(1, deck.size());
		}
		TEST_METHOD(Player_CardsPlacedCounter)
		{
			Player currentPlayer{ 6u, "Frank" };
			currentPlayer.IncrementCardsPlaced();
			currentPlayer.IncrementCardsPlaced();
			Assert::AreEqual<uint8_t>(2, currentPlayer.GetCardsPlacedRound());
			currentPlayer.ResetCardsPlaced();
			Assert::AreEqual<uint8_t>(0, currentPlayer.GetCardsPlacedRound());
		}
		TEST_METHOD(Player_DrawCards_RespectsCardsPlaced)
		{
			Pile deck;
			deck.push(Card<uint8_t>{10});
			deck.push(Card<uint8_t>{20});
			Player currentPlayer{ 7u, "Gina" };
			currentPlayer.IncrementCardsPlaced();
			currentPlayer.IncrementCardsPlaced();
			currentPlayer.DrawCards(deck);
			Assert::AreEqual<size_t>(2, currentPlayer.GetCardsCount());
			Assert::AreEqual<size_t>(0, deck.size());
		}
	};
}