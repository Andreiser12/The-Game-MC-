export module DatabaseStructures;

import <string>;

export namespace DB
{
	class User
	{
	public:
		User() = default;
		explicit User(const std::string& username) 
			: m_username{ username } , m_avatarPath{ "avatars/default.png" } { }

		inline unsigned GetID() const { return m_ID; }
		inline const std::string& GetUsername() const { return m_username; }
		inline unsigned GetGamesPlayed() const { return m_gamesPlayed; }
		inline unsigned GetGamesWon() const { return m_gamesWon; }
		inline double GetHoursPlayed() const { return m_hoursPlayed; }
		inline uint8_t GetFinalCards() const { return m_finalCards; }
		inline const std::string& GetAvatarPath() const { return m_avatarPath; }

		void SetID(unsigned id) { m_ID = id; }
		void SetUsername(const std::string& username) { m_username = username; }
		void SetGamesPlayed(unsigned gamesPlayed) { m_gamesPlayed = gamesPlayed; }
		void SetGamesWon(unsigned gamesWon) { m_gamesWon = gamesWon; }
		void SetHoursPlayed(double hours) { m_hoursPlayed = hours; }
		void SetFinalCards(uint8_t finalCards) { m_finalCards = finalCards; }
		void SetAvatarPath(const std::string& path) { m_avatarPath = path; }

	private:
		unsigned m_ID = { 0u };
		std::string m_username{ "" };
		unsigned m_gamesPlayed{ 0u };
		unsigned m_gamesWon{ 0u };
		double m_hoursPlayed{ 0.0 };
		uint8_t m_finalCards{ 0u };
		std::string m_avatarPath{ "avatars/default.png" };
	};
}
