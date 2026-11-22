#include "CppUnitTest.h"
import Deck;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheGameTests
{
	TEST_CLASS(DeckTests)
	{
	public:
		TEST_METHOD(Deck_Ascending_StartsWithOne)
		{
			Deck deck{ Deck::DeckType::Ascending, 0 };

			Assert::AreEqual<uint8_t>(Deck::ASCENDING_START_CARD, deck.GetTopCard().GetId());
		}
		TEST_METHOD(Deck_Descending_StartsWithHundred)
		{
			Deck deck{ Deck::DeckType::Descending, 1 };

			Assert::AreEqual<uint8_t>(Deck::DESCENDING_START_CARD, deck.GetTopCard().GetId());
		}
		TEST_METHOD(Deck_ReturnsCorrectType)
		{
			Deck ascending{ Deck::DeckType::Ascending, 0 };
			Deck descending{ Deck::DeckType::Descending, 1 };

			Assert::IsTrue(ascending.GetDeckType() == Deck::DeckType::Ascending);
			Assert::IsTrue(descending.GetDeckType() == Deck::DeckType::Descending);
		}
		TEST_METHOD(Deck_ReturnsCorrectIndex)
		{
			Deck deck{ Deck::DeckType::Ascending, 3 };

			Assert::AreEqual<uint8_t>(3, deck.GetDeckIndex());
		}
		TEST_METHOD(Deck_PushCard_UpdatesTopCard)
		{
			Deck deck{ Deck::DeckType::Ascending, 0 };
			deck.PlaceCard(Card<uint8_t>{42});
			Assert::AreEqual<uint8_t>(42, deck.GetTopCard().GetId());
		}
	};
}