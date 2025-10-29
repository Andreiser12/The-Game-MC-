#include <iostream>
#include "Table.h"
#include "UserProfile.h"

int main()
{
	std::stack<Cards> cards;
	Table t(3, cards);
	if (t.isDeckEmpty())
		std::cout << "There aren't cards in table." << std::endl;
	else std::cout << "There are cards in table.\n" << std::endl;

	UserProfile up=UserProfile("abcd", 2, 1, 10, 3);
	std::cout << up.PerformanceScore();

	return 0;
}