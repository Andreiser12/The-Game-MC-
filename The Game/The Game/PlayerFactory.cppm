export module PlayerFactory;

import DatabaseManager;
import DatabaseStructures;
import Player;
import <memory>;
import <optional>;
import <string>;

export class PlayerFactory
{
public:
	PlayerFactory(DatabaseManager& db);

	void Create(const std::string& username);

private:
	DatabaseManager& m_db;
};
