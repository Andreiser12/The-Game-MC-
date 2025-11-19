#include "LoginWindow.h"

LoginWindow::LoginWindow(sf::RenderWindow& window)
{
}

void LoginWindow::handleUserInput(sf::Event event)
{

}


const sf::RectangleShape& LoginWindow::getLogin() const
{
    return m_loginBox;
}

const sf::RectangleShape& LoginWindow::getUser() const
{
    return m_userBox;
}
