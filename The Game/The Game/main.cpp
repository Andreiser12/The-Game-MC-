#include <iostream>
#include "Table.h"
int main()
{
	std::stack<Cards> cards;
	Table t(3, cards);
	if (t.isDeckEmpty())
		std::cout << "There aren't cards in table." << std::endl;
	else std::cout << "There are cards in table." << std::endl;
	return 0;
}