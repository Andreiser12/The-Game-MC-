export module UserProfile;

import <string>;

export class UserProfile
{
public:
	UserProfile(std::string username = "", unsigned gamesPlayed = 0u,
		unsigned gamesWon = 0u, double hoursPlayed = 0.0, uint8_t finalCards=0u);
	~UserProfile();

	double PerformanceScore() const;
	void UpdateStats(double hours, bool win, uint8_t finalCards);

	const std::string& GetUsername() const;

private:
	std::string m_username;
	unsigned m_gamesPlayed;
	unsigned m_gamesWon;
	double m_hoursPlayed;
	uint8_t m_finalCards;

};

