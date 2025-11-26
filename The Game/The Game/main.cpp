#include <iostream>
#include <filesystem>
#include <thread>
#include <memory>

#include <crow.h>

import Table;
import Menu;
import UserProfile;
import DatabaseManager;

void TestDB()
{
	DatabaseManager database{ "game.db" };

	DB::User user;
	std::string username;
	std::cout << "Read a name for user: ";
	std::cin >> username;
	user.SetUsername(username);
	database.AddUser(user);

	unsigned int ID;
	std::cout << "Read ID for a person: ";
	std::cin >> ID;

	auto uTarget = database.GetUser(ID);
	if (uTarget)
		std::cout << *uTarget;
	else std::cout << "This user with ID " << ID << " doesn't exist in User table." << std::endl;

	DB::PlayerHand hand;
	std::cout << "Read a ID for a user: ";
	std::cin >> ID;
	hand.SetUserID(ID);
	std::string handUser;
	std::cout << "Read a hand for a user: ";
	std::cin >> handUser;
	hand.SetCards(handUser);
	database.SavePlayerHand(hand);

	std::cout << "Read ID for a user: ";
	std::cin >> ID;

	auto uHand = database.GetPlayerHand(ID);
	if (uHand)
		std::cout << *uHand;
	else std::cout << "This hand with ID " << ID << " doesn't exist in PlayerHand table." << std::endl;
}
int main()
{
	//TestDB();
	
	Table table(3);
	Menu menu(table);

	DatabaseManager database{ "gameDB.db" };
	auto Users = database.GetUsers();
	for (const auto& user : Users)
	{
		std::cout << user <<std::endl;
	}

	crow::SimpleApp app;
	CROW_ROUTE(app, "/users")([&database]() {
		std::vector<crow::json::wvalue> usersJson;
		auto users = database.GetUsers();
		for (const auto& user : users)
		{
			usersJson.push_back(crow::json::wvalue{
				{"id", user.GetID()},
				{"username", user.GetUsername()}
				});
		}
		return crow::json::wvalue{ usersJson };
	});
	app.port(18080).multithreaded().run();

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