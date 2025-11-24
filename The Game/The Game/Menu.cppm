export module Menu;

import <iostream>;

import Table;

export class Menu
{
public:
	Menu(const Table& table);

	void ShowMenu() const;
	void Choices();

private:
	Table m_table;
};

