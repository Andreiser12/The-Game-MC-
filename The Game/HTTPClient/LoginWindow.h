#pragma once
#include "SFML/Graphics.hpp"
#include <string>

class LoginWindow
{
public:
	LoginWindow(sf::RenderWindow& window);
	
	void handleUserInput(sf::Event event);
	void update();
	void draw(sf::RenderWindow& window);

	const sf::RectangleShape& getLogin() const;
	const sf::RectangleShape& getUser() const;


private:
	sf::Font m_font;
	//sf::Text m_usernameLabel;

	sf::RectangleShape m_loginBox;
	sf::RectangleShape m_userBox;

	std::string usernameInput;
};

