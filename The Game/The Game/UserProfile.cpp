module UserProfile;

UserProfile::UserProfile(std::string username, unsigned int gamesPlayed,
	unsigned int gamesWon, double hoursPlayed,
	unsigned int finalCards) : m_username{ username }, m_gamesPlayed{gamesPlayed},
	m_gamesWon{ gamesWon }, m_hoursPlayed{ hoursPlayed }, m_finalCards{finalCards} {}

UserProfile::~UserProfile() {}

double UserProfile::PerformanceScore() const
{
	if (m_finalCards == 0u)
		return 0.0;
	return static_cast<double>((m_gamesPlayed + m_gamesWon)/ m_finalCards);
}

void UserProfile::UpdateStats(double hours, bool win, unsigned int finalCards)
{
	m_hoursPlayed += hours;
	++m_gamesPlayed;
	if (win) ++m_gamesWon;
	m_finalCards = finalCards;
}

const std::string& UserProfile::GetUsername() const
{
	return m_username;
}