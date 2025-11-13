#pragma once
#include <string>

struct User
{
    unsigned int ID;
    std::string username;
};

struct UserProfileDB
{
    unsigned int ID;
    unsigned int user_ID;
    unsigned int gamesPlayed;
    unsigned int gamesWon;
    double hoursPlayed;
    unsigned int finalCards;
};

struct PlayerHand
{
    int ID;
    int user_ID;
    std::string cards;  // Current cards in player's hand
};
