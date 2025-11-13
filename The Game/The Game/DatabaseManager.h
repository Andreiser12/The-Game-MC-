#pragma once
#include "DatabaseStructures.h"
#include <sqlite_orm/sqlite_orm.h>
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
	decltype(sqlite_orm::make_storage("game.db")) m_storage;
	void initStorage();
};

