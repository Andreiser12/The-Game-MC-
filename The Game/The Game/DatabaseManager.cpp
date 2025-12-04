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

std::optional<DB::User> DatabaseManager::GetUserByID(unsigned userID)
{
    return m_storage.get_optional<DB::User>(userID);
}
std::optional<DB::User> DatabaseManager::GetUserByName(const std::string& username)
{
    auto result = m_storage.get_all<DB::User>(
        sqlite_orm::where(sqlite_orm::c(&DB::User::GetUsername) == username));
    if (!result.empty()) return result.front();
    return std::nullopt;
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


