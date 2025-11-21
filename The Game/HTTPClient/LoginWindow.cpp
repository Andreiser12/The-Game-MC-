#include "LoginWindow.h"

LoginWindow::LoginWindow(sf::RenderWindow& window) 
    : m_login(window)
{
   LoadWidgets();
}

void LoginWindow::Login(tgui::EditBox::Ptr username)
{
    std::cout << "Username: " << username->getText() << std::endl;
}

void LoginWindow::UpdateTextSize()
{
    const float windowHeight = m_login.getView().getRect().height;
    m_login.setTextSize(static_cast<unsigned int>(0.07f * windowHeight));
}

void LoginWindow::Draw()
{
    m_login.draw();
}

void LoginWindow::LoadWidgets()
{
    UpdateTextSize();

    m_login.onViewChange([this] {UpdateTextSize(); });

    auto editBoxUsername = tgui::EditBox::create();
    editBoxUsername->setSize({ "66.67%", "12.5%" });
    editBoxUsername->setPosition({ "50%", "50%" });
    editBoxUsername->setOrigin(0.5f, 0.5f);
    editBoxUsername->setDefaultText("Username");
    m_login.add(editBoxUsername);


    auto loginButton = tgui::Button::create("Login");
    loginButton->setSize({ "20%", "16.67%" });
    auto coordX = editBoxUsername->getSize().x - editBoxUsername->getPosition().x;
    auto coordY = editBoxUsername->getSize().y*1.5f + editBoxUsername->getPosition().y;
   
    loginButton->setPosition({ coordX, coordY });
    m_login.add(loginButton);
    loginButton->setTextSize(24);

    auto registerButton = tgui::Button::create("Register");
    registerButton->setSize({ "20%", "16.67%" });
    coordX = editBoxUsername->getPosition().x + loginButton->getSize().x*0.7f;
    coordY = editBoxUsername->getSize().y * 1.5f + editBoxUsername->getPosition().y;
    registerButton->setPosition({ coordX, coordY });

    m_login.add(registerButton);
    registerButton->setTextSize(24);

    loginButton->onPress([this, editBoxUsername] {
        Login(editBoxUsername);
        });
}
