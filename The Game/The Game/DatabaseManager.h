#pragma once
#include "DatabaseStructures.h"
#include <optional>
#include <iostream>
#include <sqlite_orm/sqlite_orm.h>

inline auto CreateStorage(const std::string& filename)
{
    return sqlite_orm::make_storage(filename,
        sqlite_orm::make_table("Players",
            sqlite_orm::make_column("ID", &DB::Player::ID, sqlite_orm::primary_key().autoincrement()),
            sqlite_orm::make_column("username", &DB::Player::username, sqlite_orm::unique())),
        sqlite_orm::make_table("UserProfiles",
            sqlite_orm::make_column("ID", &DB::UserProfile::ID, sqlite_orm::primary_key().autoincrement()),
            sqlite_orm::make_column("player_ID", &DB::UserProfile::player_ID),
            sqlite_orm::make_column("gamesPlayed", &DB::UserProfile::gamesPlayed),
            sqlite_orm::make_column("gamesWon", &DB::UserProfile::gamesWon),
            sqlite_orm::make_column("hoursPlayed", &DB::UserProfile::hoursPlayed),
            sqlite_orm::make_column("finalCards", &DB::UserProfile::finalCards)),
        sqlite_orm::make_table("PlayerHands",
            sqlite_orm::make_column("ID", &DB::PlayerHand::ID, sqlite_orm::primary_key().autoincrement()),
            sqlite_orm::make_column("player_ID", &DB::PlayerHand::player_ID),
            sqlite_orm::make_column("cards", &DB::PlayerHand::cards)));
}
using Storage = decltype(CreateStorage(" "));

class DatabaseManager
{
public:
	DatabaseManager(const std::string& databasePath);

	//Methods for User
	bool AddPlayer(const DB::Player& player);
    std::optional<DB::Player> GetPlayer(unsigned int ID);

	//Methods for UserProfileDB
	void AddUserProfile(const DB::UserProfile& profile);
    std::optional<DB::UserProfile> GetUserProfile(unsigned int userID);

	//Methods for PlayerHand
	void SavePlayerHand(const DB::PlayerHand& hand);
    std::optional<DB::PlayerHand> GetPlayerHand(unsigned int userID);

private:
    Storage m_storage;
};

