#pragma once
#include "DatabaseStructures.h"
#include <sqlite_orm/sqlite_orm.h>

inline auto CreateStorage(const std::string& filename)
{
    return sqlite_orm::make_storage(filename,
        sqlite_orm::make_table("Users",
            sqlite_orm::make_column("ID", &User::ID, sqlite_orm::primary_key().autoincrement()),
            sqlite_orm::make_column("username", &User::username)),
        sqlite_orm::make_table("UserProfiles",
            sqlite_orm::make_column("ID", &UserProfileDB::ID, sqlite_orm::primary_key().autoincrement()),
            sqlite_orm::make_column("user_ID", &UserProfileDB::user_ID),
            sqlite_orm::make_column("gamesPlayed", &UserProfileDB::gamesPlayed),
            sqlite_orm::make_column("gamesWon", &UserProfileDB::gamesWon),
            sqlite_orm::make_column("hoursPlayed", &UserProfileDB::hoursPlayed),
            sqlite_orm::make_column("finalCards", &UserProfileDB::finalCards)),
        sqlite_orm::make_table("PlayerHands",
            sqlite_orm::make_column("ID", &PlayerHand::ID, sqlite_orm::primary_key().autoincrement()),
            sqlite_orm::make_column("user_ID", &PlayerHand::user_ID),
            sqlite_orm::make_column("cards", &PlayerHand::cards)));
}

using Storage = decltype(CreateStorage(" "));

class DatabaseManager
{
public:
	DatabaseManager(const std::string& databasePath);

	//Methods for User
	void AddUser(const User& user);
	User GetUser(unsigned int ID) const;

	//Methods for UserProfileDB
	void AddUserProfile(const UserProfileDB& profile);
	UserProfileDB GetUserProfile(unsigned int userID) const;

	//Methods for PlayerHand
	void SavePlayerHand(const PlayerHand& hand);
	PlayerHand GetPlayerHand(unsigned int userID) const;

private:
    Storage m_storage;
};

