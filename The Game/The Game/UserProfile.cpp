#include "UserProfile.h"

UserProfile::UserProfile(std::string username, unsigned int gamesPlayed,
	unsigned int gamesWon, double hoursPlayed,
	unsigned int finalCards) : m_username{ username }, m_gamesPlayed{gamesPlayed},
	m_gamesWon{ gamesWon }, m_hoursPlayed{ hoursPlayed }, m_finalCards{finalCards} {}

UserProfile::~UserProfile() {}

double UserProfile::PerformanceScore() const
{
	return (double)(m_gamesPlayed + m_gamesWon)/ m_finalCards;
}

const std::string UserProfile::GetUsername() const
{
	return m_username;
}
