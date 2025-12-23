#pragma once
#include <vector>
#include <cstdint>
#include <optional>

#ifdef _WINDLL
#define AI_API __declspec(dllexport)
#else
#define AI_API __declspec(dllimport)
#endif

namespace AssistantAI {

    enum class DeckType : uint8_t {
        Ascending = 0,
        Descending = 1
    };

    struct DeckState {
        int index;
        uint8_t topCard;
        DeckType type;
    };

    struct MoveResult {
        int deckIndex;
        uint8_t cardValue;
        int score;
    };

    class AI_API GreedyBot 
    {
    public:
        static std::optional<MoveResult> FindBestMove(
            const std::vector<uint8_t>& myHand,
            const std::vector<DeckState>& decks,
            const std::vector<std::vector<uint8_t>>& otherHands);

        static bool IsMoveValid(uint8_t card, const DeckState& deck);
    };
}