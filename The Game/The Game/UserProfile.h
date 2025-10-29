#pragma once
#include <string>

class UserProfile
{
public:
	UserProfile(std::string username = "", unsigned int gamesPlayed = 0,
		unsigned int gamesWon = 0, double hoursPlayed = 0.0, unsigned int finalCards=0);
	~UserProfile();

	double PerformanceScore() const;

	const std::string GetUsername() const;



private:
	std::string m_username;
	unsigned int m_gamesPlayed;
	unsigned int m_gamesWon;
	double m_hoursPlayed;
	unsigned int m_finalCards;

};

