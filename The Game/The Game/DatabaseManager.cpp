module DatabaseManager;

import Player;

DatabaseManager::DatabaseManager(const std::string& databasePath)
    : m_storage{ CreateStorage(databasePath) }
{
    m_storage.sync_schema();
    auto count = m_storage.count<DB::User>();
    if (!count)
    {
        PopulateDataBase();
    }
}

bool DatabaseManager::AddUser(const DB::User& user)
{
    auto existingUser = m_storage.get_all<DB::User>(sqlite_orm::where(sqlite_orm::c(&DB::User::GetUsername) == user.GetUsername() ));
    if (!existingUser.empty())
    {
        std::cout << "User with name " << user.GetUsername() << " already exists!" << std::endl;
        return false;
    }
    m_storage.insert(user);
    std::cout << "Added user." << std::endl;
    return true;
}

std::optional<DB::User> DatabaseManager::GetUser(unsigned int userID)
{
    return m_storage.get_optional<DB::User>(userID);
}

void DatabaseManager::SavePlayerHand(const DB::PlayerHand& hand)
{
    auto existingHand = m_storage.get_optional<DB::PlayerHand>(sqlite_orm::where(sqlite_orm::c(&DB::PlayerHand::GetUserID) == hand.GetUserID()));
    if (existingHand)
    {
        m_storage.update(hand);
        std::cout << "Updated hand for user."<<std::endl;
        return;
    }
    m_storage.insert(hand);
    std::cout << "Added hand for user." << std::endl;
}

std::optional<DB::PlayerHand> DatabaseManager::GetPlayerHand(unsigned int userID)
{
    return m_storage.get_optional<DB::PlayerHand>(
        sqlite_orm::where(sqlite_orm::c(&DB::PlayerHand::GetUserID) == userID)
    );
}

void DatabaseManager::PopulateDataBase()
{
    std::vector<DB::User> users{ 
        DB::User{"ssv"}, 
        DB::User{"sssvsvfv"},
        DB::User{"svabvdshv"},
    };
    m_storage.insert_range(users.begin(), users.end());
}

std::vector<DB::User> DatabaseManager::GetUsers()
{
    return m_storage.get_all<DB::User>();
}


