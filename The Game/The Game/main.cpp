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
	
	deck = t.getDeck();
	while (!deck.empty())
	{
		std::cout<<deck.top()<<std::endl;
		deck.pop();
	}
	if (t.isDeckEmpty())
		std::cout << "There aren't cards in table.\n" << std::endl;
	else std::cout << "There are cards in table.\n" << std::endl;

	UserProfile up=UserProfile("abcd", 2, 1, 10, 3);
	std::cout << up.PerformanceScore();

	return 0;
}