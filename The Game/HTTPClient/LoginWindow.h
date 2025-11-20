#pragma once
#include "SFML/Graphics.hpp"
#include <string>

class LoginWindow
{
public:
	LoginWindow(sf::RenderWindow& window);
	
	void handleUserInput(sf::Event event);

	const sf::RectangleShape& getLogin() const;
	const sf::RectangleShape& getUser() const;
	const sf::RectangleShape& getLoginButton() const;
	const sf::RectangleShape& getRegisterButton() const;

	const sf::Text& getUserText() const;
	const sf::Text& getLoginText() const;
	const sf::Text& getRegisterText() const;

	bool isLoginPressed() const;
	bool isRegisterPressed() const;

private:


	sf::RectangleShape m_loginBox;
	sf::RectangleShape m_usernameBox;

	sf::Text m_userText;
	sf::Text m_loginText;
	sf::Text m_registerText;

	sf::Font m_font;

	bool m_textFocused = false;

	sf::RectangleShape m_loginButton;
	sf::RectangleShape m_registerButton;

	std::string m_usernameInput;
};

