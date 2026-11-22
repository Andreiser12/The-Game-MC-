module; 
#include <string> 

export module UserProfile;

export struct UserProfile
{
	std::string username;
	unsigned gamesPlayed;
	unsigned gamesWon;
	double hoursPlayed;
	std::string avatarPath;
};