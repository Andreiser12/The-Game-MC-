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

	bool IsLoggedIn(unsigned playerId) const;
	bool IsOnline(unsigned playerId, std::chrono::steady_clock::duration timeout) const;
	std::vector<unsigned> GetInactivePlayers(std::chrono::steady_clock::duration timeout) const;

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