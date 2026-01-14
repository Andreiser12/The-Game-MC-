module SessionManager;

SessionManager& SessionManager::Instance()
{
	static SessionManager instance;
	return instance;
}

bool SessionManager::Login(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto [iterator, inserted] = m_lastPlayerActivity.emplace
	(playerId, std::chrono::steady_clock::now());
	return inserted;
}

bool SessionManager::Logout(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_lastPlayerActivity.erase(playerId) == 1;
}

void SessionManager::UpdateActivity(unsigned playerId)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto iterator = m_lastPlayerActivity.find(playerId);
	if (iterator != m_lastPlayerActivity.end())
		iterator->second = std::chrono::steady_clock::now();
}

bool SessionManager::IsLoggedIn(unsigned playerId) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_lastPlayerActivity.contains(playerId);
}

bool SessionManager::IsOnline(unsigned playerId, std::chrono::steady_clock::duration timeout) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto iteratorActivity = m_lastPlayerActivity.find(playerId);
	if (iteratorActivity == m_lastPlayerActivity.end())
		return false;
	return std::chrono::steady_clock::now() - iteratorActivity->second < timeout;
}

std::vector<unsigned> SessionManager::GetInactivePlayers
(std::chrono::steady_clock::duration timeout) const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	auto now = std::chrono::steady_clock::now();
	std::vector<unsigned> inactivePlayers;
	for(const auto &[playerId, lastActivity] : m_lastPlayerActivity)
		if (now - lastActivity >= timeout)
			inactivePlayers.emplace_back(playerId);
	return inactivePlayers;
}