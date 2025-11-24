export module DatabaseStructures;

import <string>;
import <iostream>;

export namespace DB
{
    struct Player
    {
        unsigned int ID;
        std::string username;

        unsigned int GetId() const
        {
            return ID;
        }

        const std::string& GetUsername() const
        {
            return username;
        }

        friend std::ostream& operator<<(std::ostream& out, const Player& other)
        {
            out << other.ID << " " << other.username;
            return out;
        }
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
