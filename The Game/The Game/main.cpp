#include <iostream>
#include <unordered_set>

#include "Table.h"
#include "UserProfile.h"

void ShowMenu()
{
	std::cout << "\n=== Game Menu ===\n";
	std::cout << "1. Show all cards in main deck\n";
	std::cout << "2. Show top card from Ascending deck\n";
	std::cout << "3. Show top card from Descending deck\n";
	std::cout << "4. Shuffle table deck\n";
	std::cout << "5. Place a card\n";
	std::cout << "0. Exit\n";
	std::cout << "\nChoose an option: ";
}

int main()
{
	std::stack<Card> deck;

	for (unsigned int i = 2; i <= 99; i++)
		deck.push(Card(i));

	Table t(3, deck);
	t.Shuffle();
	
	if (t.isDeckEmpty())
		std::cout << "There aren't cards in table.\n" << std::endl;
	else std::cout << "There are cards in table.\n" << std::endl;

	Deck& firstAscending = t.GetDeck(Deck::DeckType::Ascending, Deck::DeckNumber::First);
	Deck& firstDescending = t.GetDeck(Deck::DeckType::Descending, Deck::DeckNumber::First);
	if (t.BackwardsTrick(firstAscending, Card(5u)))
	{
		std::cout << "Card with ID 5 can be placed on the ascending deck."<<std::endl;
	}
	else std::cout << "Card with ID 5 cannot be placed on the ascending deck."<<std::endl;
	
	if (t.BackwardsTrick(firstAscending, Card(10u)))
		std::cout << "Card with ID 10 can be placed on the ascending deck." << std::endl;
	else
		std::cout << "Card with ID 10 cannot be placed on the ascending deck." << std::endl;

	if (t.BackwardsTrick(firstDescending, Card(110u)))
		std::cout << "Card with ID 110 can be placed on the descending deck." << std::endl;
	else
		std::cout << "Card with ID 110 cannot be placed on the descending deck." << std::endl;

	if (t.BackwardsTrick(firstDescending, Card(50u)))
		std::cout << "Card with ID 50 can be placed on the descending deck." << std::endl;
	else
		std::cout << "Card with ID 50 cannot be placed on the descending deck." << std::endl;
	
	Card c1(10u);
	Card c2(5u);
	Card c3(110u);
	Card c4(50u);

	std::cout << "CheckCard for Ascending with 10: "
		<< (t.CheckCard(firstAscending, c1) ? "valid" : "invalid") << std::endl;
	std::cout << "CheckCard for Ascending with 5: "
		<< (t.CheckCard(firstAscending, c2) ? "valid" : "invalid") << std::endl;
	std::cout << "CheckCard for Descending with 110: "
		<< (t.CheckCard(firstDescending, c3) ? "valid" : "invalid") << std::endl;
	std::cout << "CheckCard for Descending with 50: "
		<< (t.CheckCard(firstDescending, c4) ? "valid" : "invalid") << std::endl;

	int option;
	do {
		ShowMenu();
		std::cin >> option;
		std::cout << std::endl;

		switch (option) {
		case 1:
		{
			std::stack<Card> temp = deck;
			std::cout << "All cards in deck:\n";
			while (!temp.empty())
			{
				std::cout << temp.top() << std::endl;
				temp.pop();
			}
			break;
		}

		case 2:
		{
			try {
				std::cout << "Top card from Ascending deck:" << firstAscending.GetTopCard() << "\n";
			}
			catch (...) {
				std::cout << "Ascending deck is empty!\n";
			}
			break;
		}

		case 3:
		{
			try {
				std::cout << "Top card from Descending deck:" << firstDescending.GetTopCard() << "\n";
			}
			catch (...) {
				std::cout << "Descending deck is empty!\n";
			}
			break;
		}
		case 4:
		{
			t.Shuffle();
			std::cout << "Deck shuffled succesfully!\n";
		}

		case 5:
		{
			unsigned int id;
			int deckChoice;
			std::cout << "Enter card ID: ";
			std::cin >> id;
			std::cout << "Choose deck (1 = Ascending, 2 = Descending): ";
			std::cin >> deckChoice;

			Card newCard(id);
			Deck& chosenDeck = (deckChoice == 1) ? firstAscending : firstDescending;

			if (t.CheckCard(chosenDeck, newCard))
				std::cout << "Card " << id << " can be placed!\n";
			else
				std::cout << "Card " << id << "cannot be placed!\n";
			break;
		}

		case 0:
		{
			std::cout << "Exiting game...\n";
			break;
		}

		default:
			std::cout << "Invalid option!\n";
			break;
		}
	}while (option != 0);

	return 0;
}