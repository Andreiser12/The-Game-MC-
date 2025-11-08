#include <iostream>
#include "Table.h"
#include "UserProfile.h"

int main()
{
	std::stack<Cards> deck;

	for (unsigned int i = 2; i <= 99; i++)
		deck.push(Cards(i));

	Table t(3, deck);
	t.Shuffle();
	
	/*while (!deck.empty())
	{
		std::cout<<deck.top()<<std::endl;
		deck.pop();
	}*/
	if (t.isDeckEmpty())
		std::cout << "There aren't cards in table.\n" << std::endl;
	else std::cout << "There are cards in table.\n" << std::endl;

	Deck& firstAscending = t.getDeck(Deck::DeckType::Ascending, Deck::DeckNumber::First);
	Deck& firstDescending = t.getDeck(Deck::DeckType::Descending, Deck::DeckNumber::First);
	if (t.BackwardsTrick(firstAscending, Cards(5u)))
	{
		std::cout << "Card with ID 5 can be placed on the ascending deck."<<std::endl;
	}
	else std::cout << "Card with ID 5 cannot be placed on the ascending deck."<<std::endl;
	
	if (t.BackwardsTrick(firstAscending, Cards(10u)))
		std::cout << "Card with ID 10 can be placed on the ascending deck." << std::endl;
	else
		std::cout << "Card with ID 10 cannot be placed on the ascending deck." << std::endl;

	if (t.BackwardsTrick(firstDescending, Cards(110u)))
		std::cout << "Card with ID 110 can be placed on the descending deck." << std::endl;
	else
		std::cout << "Card with ID 110 cannot be placed on the descending deck." << std::endl;

	if (t.BackwardsTrick(firstDescending, Cards(50u)))
		std::cout << "Card with ID 50 can be placed on the descending deck." << std::endl;
	else
		std::cout << "Card with ID 50 cannot be placed on the descending deck." << std::endl;
	
	UserProfile up=UserProfile("abcd", 2, 1, 10, 3);
	std::cout << up.PerformanceScore();

	return 0;
}