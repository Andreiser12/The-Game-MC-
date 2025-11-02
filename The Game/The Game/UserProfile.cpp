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
//0
//mutat sfml in folder-ul de proiect si inclus folder-ul in fisierul .gitignore

//1
// link-are cu lib si cai absolute

//2
// link-are cu lib si cai relatie la folderul sfml 
//ex: ../sfml etc...