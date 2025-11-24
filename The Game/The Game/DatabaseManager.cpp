module DatabaseManager;

import Player;

DatabaseManager::DatabaseManager(const std::string& databasePath)
    : m_storage{ CreateStorage(databasePath) }
{
    m_storage.sync_schema();
    auto count = m_storage.count<DB::Player>();
    if (!count)
    {
        PopulateDataBase();
    }
}

bool DatabaseManager::AddPlayer(const DB::Player& player)
{
    auto existingPlayer = m_storage.get_all<DB::Player>(sqlite_orm::where(sqlite_orm::c(&DB::Player::username) == player.username));
    if (!existingPlayer.empty())
    {
        std::cout << "Player with username " << player.username << " already exists!" << std::endl;
        return false;
    }
    m_storage.insert(player);
    std::cout << "Added player." << std::endl;
    return true;
}

std::optional<DB::Player> DatabaseManager::GetPlayer(unsigned int ID) 
{
    return m_storage.get_optional<DB::Player>(ID);
}

void DatabaseManager::PopulateDataBase()
{
    std::vector<DB::Player> players{ 
        DB::Player{ 1, "ssv"}, 
        DB::Player{ 2, "sssvsvfv"},
        DB::Player{ 3, "svabvdshv"},
    };
    m_storage.insert_range(players.begin(), players.end());
}

std::vector<DB::Player> DatabaseManager::GetPlayers()
{
    return m_storage.get_all<DB::Player>();
}


