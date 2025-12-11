module Menu;

Menu::Menu(const Table& table) :
	m_table{table} {}

void Menu::ShowMenu() const
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

void Menu::Choices()
{
	int option;
	//do {
	//	ShowMenu();
	//	std::cin >> option;
	//	std::cout << std::endl;
	//	Deck& firstAscending = m_table.GetDeck(Deck::DeckType::Ascending, Deck::DeckNumber::First);
	//	Deck& firstDescending = m_table.GetDeck(Deck::DeckType::Descending, Deck::DeckNumber::First);
	//	switch (option) {
	//	case 1:
	//	{
	//		std::stack<Card<uint16_t>> temp = m_table.GetMainDeck();
	//		std::cout << "All cards in deck:\n";
	//		while (!temp.empty())
	//		{
	//			std::cout << temp.top() << std::endl;
	//			temp.pop();
	//		}
	//		break;
	//	}

	//	case 2:
	//	{
	//		try {
	//			std::cout << "Top card from Ascending deck:" << firstAscending.GetTopCard() << "\n";
	//		}
	//		catch (...) {
	//			std::cout << "Ascending deck is empty!\n";
	//		}
	//		break;
	//	}

	//	case 3:
	//	{
	//		try {
	//			std::cout << "Top card from Descending deck:" << firstDescending.GetTopCard() << "\n";
	//		}
	//		catch (...) {
	//			std::cout << "Descending deck is empty!\n";
	//		}
	//		break;
	//	}
	//	case 4:
	//	{
	//		m_table.Shuffle();
	//		std::cout << "Deck shuffled succesfully!\n";
	//		break;
	//	}

	//	case 5:
	//	{
	//		unsigned int id;
	//		int deckChoice;
	//		std::cout << "Enter card ID: ";
	//		std::cin >> id;
	//		std::cout << "Choose deck (1 = Ascending, 2 = Descending): ";
	//		std::cin >> deckChoice;

	//		Card newCard(id);
	//		Deck& chosenDeck = (deckChoice == 1) ? firstAscending : firstDescending;

	//		//if (m_table.CheckCard(chosenDeck, newCard))
	//			//std::cout << "Card " << id << " can be placed!\n";
	//		//else
	//			//std::cout << "Card " << id << "cannot be placed!\n";
	//		break;
	//	}

	//	case 0:
	//	{
	//		std::cout << "Exiting game...\n";
	//		break;
	//	}

	//	default:
	//		std::cout << "Invalid option!\n";
	//		break;
	//	}
	//} while (option != 0);
}
