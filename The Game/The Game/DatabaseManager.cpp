module DatabaseManager;

DatabaseManager::DatabaseManager(const std::string& databasePath)
	: m_storage{ std::move(CreateStorage(databasePath)) }
{
	m_storage.sync_schema();
}

bool DatabaseManager::AddUser(const DB::User& user)
{
	if (m_storage.count<DB::User>(
		sqlite_orm::where(sqlite_orm::c(&DB::User::GetUsername) == user.GetUsername())) == 0)
	{
		m_storage.insert(user);
		return true;
	}
	return false;
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

std::vector<DB::User> DatabaseManager::GetUsers()
{
	return m_storage.get_all<DB::User>();
}