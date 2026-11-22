#include "CppUnitTest.h"
#include <utility>
#include <string_view>
import GreedyAI;
import Deck;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheGameTests
{
	TEST_CLASS(HeuristicTreeTests)
	{
	public:
		TEST_METHOD(HeuristicTree_LocalScore_AbsoluteDifference)
		{
			Deck deck{ Deck::DeckType::Ascending, 0 };
			deck.PlaceCard(Card<uint8_t>{30});
			AIMove move{ deck, Card<uint8_t>{40}, 0 };
			Assert::AreEqual(10, HeuristicTree::LocalScore(move));
		}
		TEST_METHOD(HeuristicTree_LocalScore_InverseOrder)
		{
			Deck deck{ Deck::DeckType::Ascending, 0 };
			deck.PlaceCard(Card<uint8_t>{40});
			AIMove move{ deck, Card<uint8_t>{30}, 0 };
			Assert::AreEqual(10, HeuristicTree::LocalScore(move));
		}
		TEST_METHOD(HeuristicTree_BackwardsBonus_ExactTenDifference)
		{
			Deck deck{ Deck::DeckType::Ascending, 0 };
			deck.PlaceCard(Card<uint8_t>{50});
			AIMove move{ deck, Card<uint8_t>{40}, 0 };
			Assert::AreEqual(10, HeuristicTree::BackwardsBonus(move));
		}
		TEST_METHOD(HeuristicTree_BackwardsBonus_NoBonus)
		{
			Deck deck{ Deck::DeckType::Ascending, 0 };
			deck.PlaceCard(Card<uint8_t>{50});
			AIMove move{ deck, Card<uint8_t>{45}, 0 };
			Assert::AreEqual(0, HeuristicTree::BackwardsBonus(move));
		}
		TEST_METHOD(HeuristicTree_RiskPenalty_LargeGap)
		{
			Deck deck{ Deck::DeckType::Ascending, 0 };
			deck.PlaceCard(Card<uint8_t>{10});
			AIMove move{ deck, Card<uint8_t>{50}, 0 };
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{1u, "TestPlayer"}
			};
			Table dummyTable{ lobbyPlayers };
			Assert::AreEqual(20, HeuristicTree::RiskPenalty(move, dummyTable));
		}
		TEST_METHOD(HeuristicTree_RiskPenalty_SmallGap)
		{
			Deck deck{ Deck::DeckType::Ascending, 0 };
			deck.PlaceCard(Card<uint8_t>{10});
			AIMove move{ deck, Card<uint8_t>{15}, 0 };
			static const std::pair<unsigned, std::string_view> lobbyPlayers[]{
				{5u, "TestPlayer"}
			};
			Table dummyTable{ lobbyPlayers };
			Assert::AreEqual(0, HeuristicTree::RiskPenalty(move, dummyTable));
		}
	};
}