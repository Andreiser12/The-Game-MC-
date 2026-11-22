#include "CppUnitTest.h"
#include <vector>
import AssistantAI;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheGameTests
{
	TEST_CLASS(AssistantAITests)
	{
	public:
		TEST_METHOD(AssistantAI_NoValidMoves_ReturnsNullopt)
		{
			std::vector<uint8_t> myHand{ 5,6 };
			std::vector<AssistantAI::DeckState> decks{
				{0, 50, AssistantAI::DeckType::Ascending}
			};
			std::vector<std::vector<uint8_t>> otherHands{};
			auto result = AssistantAI::GreedyBot::FindBestMove(myHand, decks, otherHands);
			Assert::IsFalse(result.has_value());
		}
		TEST_METHOD(AssistantAI_OneValidMove_ReturnsIt)
		{
			std::vector<uint8_t> myHand{ 30 };
			std::vector<AssistantAI::DeckState> decks{
				{1, 25, AssistantAI::DeckType::Ascending}
			};
			std::vector<std::vector<uint8_t>> otherHands{};
			auto result = AssistantAI::GreedyBot::FindBestMove(myHand, decks, otherHands);
			Assert::IsTrue(result.has_value());
			Assert::AreEqual<uint8_t>(30, result->cardValue);
			Assert::AreEqual<int>(1, result->deckIndex);
		}
		TEST_METHOD(AssistantAI_MultipleMoves_ChoosesBestScore)
		{
			std::vector<uint8_t> myHand{ 30, 40 };
			std::vector<AssistantAI::DeckState> decks{
				{0, 28, AssistantAI::DeckType::Ascending}
			};
			std::vector<std::vector<uint8_t>> otherHands{};
			auto result = AssistantAI::GreedyBot::FindBestMove(myHand, decks, otherHands);
			Assert::IsTrue(result.has_value());
			Assert::AreEqual<uint8_t>(30, result->cardValue);
		}
	};
}