#include <iostream>
#include <unordered_set>
#include "Table.h"
#include "Menu.h"
#include "UserProfile.h"
#include "DatabaseManager.h"

void TestDB()
{
	DatabaseManager database{ "game.db" };

	DB::Player p;
	std::cout << "Read username for player: ";
	std::cin >> p.username;
	database.AddPlayer(p);

	unsigned int ID;
	std::cout << "Read ID for a person: ";
	std::cin >> ID;

	auto pTarget = database.GetPlayer(ID);
	if (pTarget)
		std::cout << "Found player with ID: " << pTarget->ID << " and name: " << pTarget->username << std::endl;
	else std::cout << "This player with ID " << ID << " doesn't exist in Player table."<<std::endl;
}
int main()
{
	Table table(3);
	Menu menu(table);

	TestDB();

	Deck& firstAscending = table.GetDeck(Deck::DeckType::Ascending, Deck::DeckNumber::First);
	Deck& firstDescending = table.GetDeck(Deck::DeckType::Descending, Deck::DeckNumber::First);
	if (table.BackwardsTrick(firstAscending, Card(5u)))
	{
		std::cout << "Card with ID 5 can be placed on the ascending deck."<<std::endl;
	}
	else std::cout << "Card with ID 5 cannot be placed on the ascending deck."<<std::endl;
	
	if (table.BackwardsTrick(firstAscending, Card(10u)))
		std::cout << "Card with ID 10 can be placed on the ascending deck." << std::endl;
	else
		std::cout << "Card with ID 10 cannot be placed on the ascending deck." << std::endl;

	if (table.BackwardsTrick(firstDescending, Card(110u)))
		std::cout << "Card with ID 110 can be placed on the descending deck." << std::endl;
	else
		std::cout << "Card with ID 110 cannot be placed on the descending deck." << std::endl;

	if (table.BackwardsTrick(firstDescending, Card(50u)))
		std::cout << "Card with ID 50 can be placed on the descending deck." << std::endl;
	else
		std::cout << "Card with ID 50 cannot be placed on the descending deck." << std::endl;
	
	Card c1(10u);
	Card c2(5u);
	Card c3(110u);
	Card c4(50u);

	std::cout << "CheckCard for Ascending with 10: "
		<< (table.CheckCard(firstAscending, c1) ? "valid" : "invalid") << std::endl;
	std::cout << "CheckCard for Ascending with 5: "
		<< (table.CheckCard(firstAscending, c2) ? "valid" : "invalid") << std::endl;
	std::cout << "CheckCard for Descending with 110: "
		<< (table.CheckCard(firstDescending, c3) ? "valid" : "invalid") << std::endl;
	std::cout << "CheckCard for Descending with 50: "
		<< (table.CheckCard(firstDescending, c4) ? "valid" : "invalid") << std::endl;

	menu.Choices();

	return 0;
}