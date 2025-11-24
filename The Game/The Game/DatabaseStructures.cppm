export module DatabaseStructures;

import <string>;

export namespace DB
{
    struct Player
    {
        unsigned int ID;
        std::string username;
    };

    struct UserProfile
    {
        unsigned int ID;
        unsigned int player_ID;
        unsigned int gamesPlayed;
        unsigned int gamesWon;
        double hoursPlayed;
        unsigned int finalCards;
    };

    struct PlayerHand
    {
        int ID;
        int player_ID;
        std::string cards;  // Current cards in player's hand
    };
}
