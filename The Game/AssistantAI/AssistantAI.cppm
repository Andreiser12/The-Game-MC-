module;

#include <vector>
#include <cstdint>
#include <optional>
#include <cmath>
#include <algorithm>
#include <ranges>

export module AssistantAI;

#ifdef ASSISTANTAI_EXPORTS
#define ASSISTANTAI_API __declspec(dllexport)
#else
#define ASSISTANTAI_API __declspec(dllimport)
#endif

namespace AssistantAI {

	export enum class DeckType : uint8_t {
		Ascending = 0,
		Descending = 1
	};

	export struct DeckState {
		int id;
		uint8_t topCard;
		DeckType type;
	};

	export struct MoveResult {
		int deckId;
		uint8_t cardValue;
		float score;
	};

	export struct HandContext {
		std::vector<uint8_t> myHand;
		std::vector<DeckState> decks;
		std::vector<std::vector<uint8_t>> otherHands;
	};

	export class __declspec(dllexport) GreedyBot
	{
	public:

		static float ScoreDistance(uint8_t card, const DeckState& deck)
		{
			int gap{ std::abs(static_cast<int>(card) - static_cast<int>(deck.topCard)) };

			if (gap == 10) return -BACKWARDS_TRICK_WEIGHT;

			return static_cast<float>(gap);
		}

		static float ScoreFlexibility(uint8_t card, const DeckState& deck, const HandContext& handContext)
		{
			DeckState simulatedDeck = deck;
			simulatedDeck.topCard = card;

			int playableCardsRemaining{ 0 };
			for (uint8_t otherCard : handContext.myHand)
			{
				if (otherCard != card && IsMoveValid(otherCard, simulatedDeck))
				{
					playableCardsRemaining++;
				}
			}

			return -static_cast<float>(playableCardsRemaining) * PLAYABLE_CARDS_WEIGHT;
		}

		static float ScoreDiversity(uint8_t card, const DeckState& deck, const HandContext& handContext)
		{
			std::vector<int> deckPlayability(handContext.decks.size(), 0);
			for (size_t i = 0; i < handContext.decks.size(); ++i)
			{
				for (uint8_t myCard : handContext.myHand)
				{
					if (IsMoveValid(myCard, handContext.decks[i]))
					{
						deckPlayability[i]++;
					}
				}
			}
			int currentDeckOptions{ deckPlayability[deck.id] };
			int minOtherOptions{ *std::min_element(deckPlayability.begin(), deckPlayability.end()) };

			if (currentDeckOptions > minOtherOptions * 2)
			{
				return 5.0f;
			}
			return 0.0f;
		}

		static float ScoreLookAhead(uint8_t card, const DeckState& deck, const HandContext& handContext)
		{
			DeckState simulatedDeck = deck;
			simulatedDeck.topCard = card;

			std::vector<uint8_t> remainingHand;
			for (uint8_t myCard : handContext.myHand)
			{
				if (myCard != card) remainingHand.emplace_back(myCard);
			}

			int validMovesAfter{ 0 };
			for (uint8_t nextCard : remainingHand)
			{
				bool canPlay{ false };
				for (const auto& deck : handContext.decks)
				{
					DeckState testDeck = (deck.id == deck.id) ? simulatedDeck : deck;
					if (IsMoveValid(nextCard, testDeck))
					{
						canPlay = true;
						break;
					}
				}
				if (canPlay) validMovesAfter++;
			}

			float futureFlexibility = static_cast<float>(validMovesAfter) /
				std::max(1, static_cast<int>(remainingHand.size()));
			return -futureFlexibility * FUTURE_FLEXIBILITY_WEIGHT;
		}

		static bool IsMoveValid(uint8_t card, const DeckState& deck)
		{
			if (deck.type == DeckType::Ascending)
			{
				return (card == deck.topCard - 10) || (card > deck.topCard);
			}
			return (card == deck.topCard + 10) || (card < deck.topCard);
		}

		static std::optional<MoveResult> FindBestMove(
			const std::vector<uint8_t>& myHand,
			const std::vector<DeckState>& decks,
			const std::vector<std::vector<uint8_t>>& otherHands = {})
		{
			if (myHand.empty() || decks.empty()) return std::nullopt;

			HandContext handContext{ myHand, decks, otherHands };
			std::vector<MoveResult> validMoves;

			for (uint8_t card : myHand)
			{
				for (const auto& deck : decks)
				{
					if (!IsMoveValid(card, deck)) continue;

					float score{ 0.0f };
					score += SCORE_DISTANCE_WEIGHT * ScoreDistance(card, deck);
					score += SCORE_FLEXIBILITY_WEIGHT * ScoreFlexibility(card, deck, handContext);
					score += SCORE_DIVERSITY_WEIGHT * ScoreDiversity(card, deck, handContext);
					score += SCORE_LOOK_AHEAD_WEIGHT * ScoreLookAhead(card, deck, handContext);

					validMoves.push_back({ deck.id, card, score });
				}
			}

			if (validMoves.empty()) return std::nullopt;

			auto best = std::min_element(validMoves.begin(), validMoves.end(),
				[](const MoveResult& a, const MoveResult& b) {
					return a.score < b.score;
				});

			return *best;
		}

	public:

		static constexpr float SCORE_DISTANCE_WEIGHT{ 1.0f };
		static constexpr float SCORE_FLEXIBILITY_WEIGHT{ 1.5f };
		static constexpr float SCORE_DIVERSITY_WEIGHT{ 0.5f };
		static constexpr float SCORE_LOOK_AHEAD_WEIGHT{ 2.0f };
		static constexpr float BACKWARDS_TRICK_WEIGHT{ 20.0f };
		static constexpr float PLAYABLE_CARDS_WEIGHT{ 3.0f };
		static constexpr float FUTURE_FLEXIBILITY_WEIGHT{ 10.0f };
	};
};




