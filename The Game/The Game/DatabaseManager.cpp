#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& databasePath)
    : m_storage{ CreateStorage(databasePath) }
{
    m_storage.sync_schema();
}

