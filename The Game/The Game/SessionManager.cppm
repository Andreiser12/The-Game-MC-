export module SessionManager;

import <vector>;
import <chrono>;
import <mutex>;
import <unordered_map>;

export class SessionManager
{
public:
	static SessionManager& Instance();
	bool Login(unsigned playerId);
	bool Logout(unsigned playerId);
	void UpdateActivity(unsigned playerId);
	bool IsOnline(unsigned playerId) const;
	std::vector<unsigned> GetInactivePlayers(std::chrono::seconds timeout) const;
private:
	SessionManager() = default;
	~SessionManager() = default;

	SessionManager(const SessionManager&) = delete;
	SessionManager& operator= (const SessionManager&) = delete;

	SessionManager(SessionManager&&) = delete;
	SessionManager& operator=(SessionManager&&) = delete;

private:
	mutable std::mutex m_mutex;
	std::unordered_map<unsigned, std::chrono::steady_clock::time_point> m_lastPlayerActivity;
};