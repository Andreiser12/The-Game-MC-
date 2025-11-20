#include "LoginWindow.h"

LoginWindow::LoginWindow(sf::RenderWindow& window) 
    :
    m_userText(m_font),
    m_loginText(m_font),
    m_registerText(m_font)
{
    m_font.openFromFile("arial.ttf");
    
    m_usernameBox.setSize({300.f, 50.f});
    m_usernameBox.setFillColor(sf::Color(220, 220, 220));
    m_usernameBox.setOutlineThickness(2.f);
    m_usernameBox.setOutlineColor(sf::Color::Black);

    float centerX = window.getPosition().x / 2.f - m_usernameBox.getSize().x / 2.f;
    float centerY = window.getPosition().y / 2.f + m_usernameBox.getSize().y / 2.f;
    m_usernameBox.setPosition({ centerX, centerY });

    m_userText.setString("Username");
    m_userText.setCharacterSize(24);
    m_userText.setFillColor(sf::Color::Black);
    
    float centerUserX = m_usernameBox.getPosition().x +
        m_usernameBox.getSize().x / 2.f;
    float centerUserY = m_usernameBox.getPosition().y +
        m_usernameBox.getSize().y / 2.f;
    m_userText.setOrigin({
        m_userText.getCharacterSize() * m_userText.getString().getSize() / 4.f,
        m_userText.getCharacterSize() / 2.f});
    m_userText.setPosition({centerUserX, centerUserY});

    m_loginText.setString("Login");
    m_loginText.setCharacterSize(20);
    m_loginText.setFillColor(sf::Color::Black);

    m_registerText.setString("Register");
    m_registerText.setCharacterSize(20);
    m_registerText.setFillColor(sf::Color::Black);

    float margin = 50.f;

    m_loginButton.setSize({125.f, 75.f});
    m_loginButton.setOutlineThickness(2.f);
    m_loginButton.setOutlineColor(sf::Color::Black);
    m_loginButton.setFillColor(sf::Color(150, 200, 255));
    m_loginButton.setPosition({
        margin, window.getSize().y - m_loginButton.getSize().y - margin });

    m_registerButton.setSize({ 125.f, 75.f });
    m_registerButton.setFillColor(sf::Color(150, 200, 255));
    m_registerButton.setOutlineThickness(2.f);
    m_registerButton.setOutlineColor(sf::Color::Black);
    m_registerButton.setPosition({
        window.getSize().x - m_registerButton.getSize().x - margin,
        window.getSize().y - m_registerButton.getSize().y - margin});

    float loginCenterX = m_loginButton.getPosition().x + m_loginButton.getSize().x / 2.f;
    float loginCenterY = m_loginButton.getPosition().y + m_loginButton.getSize().y / 2.f;
    m_loginText.setOrigin({ m_loginText.getCharacterSize() *
        m_loginText.getString().getSize() / 4.f,
        m_loginText.getCharacterSize() / 2.f });
    m_loginText.setPosition({ loginCenterX, loginCenterY });

    float registerCenterX = m_registerButton.getPosition().x
        + m_registerButton.getSize().x / 2.f;
    float registerCenterY = m_registerButton.getPosition().y
        + m_registerButton.getSize().y / 2.f;
    m_registerText.setOrigin({ m_registerText.getCharacterSize() *
        m_registerText.getString().getSize() / 4.f,
        m_registerText.getCharacterSize() / 2.f});
    m_registerText.setPosition({ registerCenterX, registerCenterY });
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
    return m_usernameBox;
}

const sf::RectangleShape& LoginWindow::getLoginButton() const
{
    return m_loginButton;
}

const sf::RectangleShape& LoginWindow::getRegisterButton() const
{
    return m_registerButton;
}

const sf::Text& LoginWindow::getUserText() const
{
    return m_userText;
}

const sf::Text& LoginWindow::getLoginText() const
{
    return m_loginText;
}

const sf::Text& LoginWindow::getRegisterText() const
{
    return m_registerText;
}

bool LoginWindow::isLoginPressed() const
{
    return false;
}

bool LoginWindow::isRegisterPressed() const
{
    return false;
}