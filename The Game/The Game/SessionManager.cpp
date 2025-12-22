module SessionManager;

SessionManager& SessionManager::Instance()
{
	static SessionManager instance;
	return instance;
}

bool SessionManager::Login(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto [iterator, inserted] = m_lastPlayerActivity.emplace(playerId, std::chrono::steady_clock::now());
	return inserted;
}

bool SessionManager::Logout(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_lastPlayerActivity.erase(playerId) == 1;
}

