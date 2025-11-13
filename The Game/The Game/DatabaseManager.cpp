#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& databasePath)
    : m_storage(sqlite_orm::make_storage(databasePath,
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
            sqlite_orm::make_column("cards", &PlayerHand::cards))))
{
    m_storage.sync_schema();
}

