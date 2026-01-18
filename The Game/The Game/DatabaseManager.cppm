export module DatabaseManager;

import DatabaseStructures;
import Player;

import <optional>;
import <string>;
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
			sqlite_orm::make_column("finalCards", &DB::User::GetFinalCards, &DB::User::SetFinalCards),
			sqlite_orm::make_column("avatarPath", &DB::User::GetAvatarPath, &DB::User::SetAvatarPath)));
}
export using Storage = decltype(CreateStorage(" "));

export class DatabaseManager
{
public:
	explicit DatabaseManager(const std::string& databasePath);

	bool AddUser(const DB::User& user);
	bool UpdateStats(unsigned userId, bool gamePlayed, bool gameWon, double hoursPlayed);
	bool DeleteUser(unsigned userId);
	bool UpdateAvatar(unsigned userId, const std::string& avatarPath);
	std::optional<DB::User> GetUserByID(unsigned userID);
	std::optional<DB::User> GetUserByName(const std::string& username);
	[[nodiscard]] std::vector<DB::User> GetUsers();

private:
	decltype(CreateStorage("")) m_storage;
};

