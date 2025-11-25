export module DatabaseManager;

import DatabaseStructures;
import Player;

import <optional>;
import <iostream>;
import <vector>;
import <sqlite_orm/sqlite_orm.h>;

export inline auto CreateStorage(const std::string& filename)
{
    return sqlite_orm::make_storage(filename,
        sqlite_orm::make_table("User",
            sqlite_orm::make_column("ID", &DB::User::GetID, &DB::User::SetID, sqlite_orm::primary_key().autoincrement()),
            sqlite_orm::make_column("username", &DB::User::GetUsername, &DB::User::SetUsername),
            sqlite_orm::make_column("gamesPlayed", &DB::User::GetGamesPlayed, &DB::User::SetGamesPlayed),
            sqlite_orm::make_column("gamesWon", &DB::User::GetGamesWon, &DB::User::SetGamesWon),
            sqlite_orm::make_column("hoursPlayed", &DB::User::GetHoursPlayed, &DB::User::SetHoursPlayed),
            sqlite_orm::make_column("finalCards", &DB::User::GetFinalCards, &DB::User::SetFinalCards)),
        sqlite_orm::make_table("PlayerHand",
            sqlite_orm::make_column("ID", &DB::PlayerHand::GetID, &DB::PlayerHand::SetID, sqlite_orm::primary_key().autoincrement()),
            sqlite_orm::make_column("user_ID", &DB::PlayerHand::GetUserID, &DB::PlayerHand::SetUserID),
            sqlite_orm::make_column("cards", &DB::PlayerHand::GetCards, &DB::PlayerHand::SetCards)));
}
export using Storage = decltype(CreateStorage(" "));

export class DatabaseManager
{
public:
	DatabaseManager(const std::string& databasePath);

	//Methods for User
	bool AddUser(const DB::User& user);
    std::optional<DB::User> GetUser(unsigned int userID);

	//Methods for PlayerHand
	void SavePlayerHand(const DB::PlayerHand& hand);
    std::optional<DB::PlayerHand> GetPlayerHand(unsigned int userID);

    void PopulateDataBase();

    std::vector<DB::User> GetUsers();

private:
    Storage m_storage;
};

