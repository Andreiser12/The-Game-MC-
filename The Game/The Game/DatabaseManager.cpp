#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& databasePath)
    : m_storage{ CreateStorage(databasePath) }
{
    m_storage.sync_schema();
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

