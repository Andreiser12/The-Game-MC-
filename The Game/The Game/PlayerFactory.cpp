module PlayerFactory;

PlayerFactory::PlayerFactory(DatabaseManager& db) : m_db(db) {}

void PlayerFactory::Create(const std::string& username)
{
	//auto userOpt = m_db.GetUserByName(username);
	//if (!userOpt)
	//{
	//	return nullptr;
	//}
	//const DB::User& user = *userOpt;

	//auto player = std::make_shared<Player>(user.GetID(), user.GetUsername());
	//return player;
}