#include "CppUnitTest.h"
#include <unordered_set>
import Card;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TheGameTests
{
	TEST_CLASS(CardTests)
	{
	public:
		TEST_METHOD(Card_StoresId)
		{
			Card<uint8_t> card{ 42 };

			Assert::AreEqual<uint8_t>(42, card.GetId());
		}
		TEST_METHOD(Card_Equality)
		{
			Card<uint8_t> card1{ 10 };
			Card<uint8_t> card2{ 10 };
			Card<uint8_t> card3{ 11 };

			Assert::IsTrue(card1 == card2);
			Assert::IsFalse(card1 == card3);
		}
		TEST_METHOD(Card_Hash_WorksInUnorderedSet)
		{
			std::unordered_set<Card<uint8_t>> cards;
			cards.emplace(5);
			cards.emplace(5);
			cards.emplace(6);

			Assert::AreEqual<size_t>(2, cards.size());
		}
	};
}