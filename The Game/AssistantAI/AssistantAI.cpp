#include "AssistantAI.h"
#include <cmath>
#include <algorithm>
#include <limits>
#include <ranges>

namespace AssistantAI {

    static constexpr int LOCAL_WEIGHT{ 1 };
    static constexpr int GLOBAL_WEIGHT{ 2 };
    static constexpr int RISK_WEIGHT{ 1 };

    int LocalScore(uint8_t card, const DeckState& deck)
    {
        return std::abs(static_cast<int>(card) - static_cast<int>(deck.topCard));
    }

    int BackwardsBonus(uint8_t card, const DeckState& deck)
    {
        if (std::abs(static_cast<int>(card) - static_cast<int>(deck.topCard)) == 10)
        {
            return 10;
        }
        return 0;
    }

    int RiskPenalty(uint8_t card, const DeckState& deck)
    {
        int gap = std::abs(static_cast<int>(card) - static_cast<int>(deck.topCard));
        if (gap >= 30) return 20;
        else if (gap >= 20) return 10;
        return 0;
    }

    int GlobalPenalty(const DeckState& targetDeck, const std::vector<std::vector<uint8_t>>& otherHands)
    {
        int penalty = 0;
        for (const auto& hand : otherHands)
        {
            int validMoves = 0;
            for (uint8_t card : hand)
            {
                if (GreedyBot::IsMoveValid(card, targetDeck))
                {
                    validMoves++;
                }
            }

            if (validMoves == 0) penalty += 50;
            else if (validMoves <= 2) penalty += 20;
            else if (validMoves <= 5) penalty += 10;
        }
        return penalty;
    }

    bool GreedyBot::IsMoveValid(uint8_t card, const DeckState& deck)
    {
        if (deck.type == DeckType::Ascending)
        {
            if (card == deck.topCard - 10) return true;
            return card > deck.topCard;
        }
        else
        {
            if (card == deck.topCard + 10) return true;
            return card < deck.topCard;
        }
    }

    std::optional<MoveResult> GreedyBot::FindBestMove(
        const std::vector<uint8_t>& myHand,
        const std::vector<DeckState>& decks,
        const std::vector<std::vector<uint8_t>>& otherHands)
    {
        std::vector<MoveResult> validMoves;

        for (uint8_t card : myHand)
        {
            for (const auto& deck : decks)
            {
                if (IsMoveValid(card, deck))
                {
                    int score = 0;
                    score += LOCAL_WEIGHT * LocalScore(card, deck);
                    score -= BackwardsBonus(card, deck);
                    score += RISK_WEIGHT * RiskPenalty(card, deck);

                    if (!otherHands.empty())
                    {
                        score += GLOBAL_WEIGHT * GlobalPenalty(deck, otherHands);
                    }

                    validMoves.push_back({ deck.index, card, score });
                }
            }
        }

        if (validMoves.empty())
        {
            return std::nullopt;
        }

        auto best = std::ranges::min_element(validMoves, {}, &MoveResult::score);
        return *best;
    }
}