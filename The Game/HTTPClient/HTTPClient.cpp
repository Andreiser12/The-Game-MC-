#include <iostream>
#include <string>

#include "framework.h"
#include "LoginWindow.h"
#include "HTTPClient.h"
#include "SFML/Graphics.hpp"

#define MAX_LOADSTRING 100

constexpr static int LOGIN_HEIGHT{ 480 };
constexpr static int LOGIN_WIDTH{ 640 };

sf::RectangleShape createRectangle(const sf::Vector2f& size, float thickness)
{
    sf::RectangleShape rect(size);
    rect.setOutlineThickness(thickness);
    return rect;
}

HINSTANCE hInst;                             
WCHAR szTitle[MAX_LOADSTRING];                
WCHAR szWindowClass[MAX_LOADSTRING];            

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    

    sf::VideoMode loginMode = sf::VideoMode::getDesktopMode();
    loginMode.size = { LOGIN_WIDTH, LOGIN_HEIGHT };
    sf::RenderWindow window(loginMode, "TheGame - Login");
    window.setFramerateLimit(60);
    LoginWindow loginWindow(window);

    while (window.isOpen())
    {
        while (true)
        {
            auto eventOpt = window.pollEvent();
            if (!eventOpt.has_value())
                break;

            const sf::Event& event = eventOpt.value();


            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
        }

        sf::RectangleShape usernameBox = loginWindow.getUser();
        sf::RectangleShape loginButton = loginWindow.getLoginButton();
        sf::RectangleShape registerButton = loginWindow.getRegisterButton();
        sf::Text userText = loginWindow.getUserText();
        sf::Text loginText = loginWindow.getLoginText();
        sf::Text registerText = loginWindow.getRegisterText();
  
        window.clear(sf::Color::White);
        window.draw(usernameBox);
        window.draw(userText);
        window.draw(loginButton);
        window.draw(loginText);
        window.draw(registerButton);
        window.draw(registerText);
        window.display();
    }
}
