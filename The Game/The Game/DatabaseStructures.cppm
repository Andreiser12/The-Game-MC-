export module DatabaseStructures;

import <string>;
import <iostream>;

export namespace DB
{
    class User
    {
        private:
            unsigned int m_ID=0u;
            std::string m_username="";
            unsigned int m_gamesPlayed=0u;
            unsigned int m_gamesWon=0u;
            double m_hoursPlayed=0.0;
            unsigned int m_finalCards=0u;
        public:
            User() = default;
            User(const std::string& username)
                : m_username{ username } {}

            inline unsigned int GetID() const { return m_ID; }
            inline std::string GetUsername() const {  return m_username; }
            inline unsigned int GetGamesPlayed() const { return m_gamesPlayed; }
            inline unsigned int GetGamesWon() const { return m_gamesWon; }
            inline double GetHoursPlayed() const { return m_hoursPlayed; }
            inline unsigned int GetFinalCards() const { return m_finalCards; }

            void SetID(unsigned int id) { m_ID = id; }
            void SetUsername(const std::string& username) { m_username = username; }
            void SetGamesPlayed(unsigned int gamesPlayed) { m_gamesPlayed = gamesPlayed; }
            void SetGamesWon(unsigned int gamesWon) { m_gamesWon = gamesWon; }
            void SetHoursPlayed(double hours) { m_hoursPlayed = hours; }
            void SetFinalCards(unsigned int finalCards) { m_finalCards = finalCards; }

            friend std::ostream& operator<<(std::ostream& out, const User& user)
            {
                out << "User with ID " << user.GetID() << ", with name " << user.GetUsername()
                    << " have the following details:" << std::endl;
                out << "Games Played: " << user.GetGamesPlayed() << std::endl;
                out << "Games Won: " << user.GetGamesWon() << std::endl;
                out << "Hours Played: " << user.GetHoursPlayed() << std::endl;
                out << "Final Cards: " << user.GetFinalCards() << std::endl;
                out << std::endl;
                return out;
            }
    };

    class PlayerHand
    {
        private:
            unsigned int m_ID=0u;
            unsigned int m_userID=0u;
            std::string m_cards="";  // Current cards in player's hand
        public:
            PlayerHand() = default;
            PlayerHand(unsigned int userID, const std::string& cards = "")
                : m_userID{ userID }, m_cards{ cards } {
            }

            inline unsigned int GetID() const { return m_ID; }
            inline unsigned int GetUserID() const { return m_userID; }
            inline std::string GetCards() const { return m_cards; }

            void SetID(unsigned int id) { m_ID = id; }
            void SetUserID(unsigned int userID) { m_userID = userID; }
            void SetCards(const std::string& cards) { m_cards = cards; }

            void AddCard(const std::string& card) { m_cards += card; }
            void ClearHand() { m_cards.clear(); }

            friend std::ostream& operator<<(std::ostream& out, const PlayerHand& hand)
            {
                out << "Playerhand with ID " << hand.GetID() << " of the user with ID " << hand.GetUserID() 
                    << " have the following cards:" << std::endl;
                out << hand.GetCards() << std::endl;
                out << std::endl;
                return out;
            }
    };
}
