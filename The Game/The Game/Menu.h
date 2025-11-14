#pragma once
#include "Table.h"
#include <iostream>

class Menu
{
public:
	Menu(const Table& table);

	void ShowMenu() const;
	void Choices();

private:
	Table m_table;
};

