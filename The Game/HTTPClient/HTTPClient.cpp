// HTTPClient.cpp : Defines the entry point for the application.
//

#include "framework.h"
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

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
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
        sf::RectangleShape loginRectangle = createRectangle({ 350.f, 150.f }, 3.f);
        loginRectangle.setOutlineColor(sf::Color::Black);
        loginRectangle.setFillColor(sf::Color::Blue);
        float radiusXLogin = loginRectangle.getSize().x/2.f;
        float radiusYLogin = loginRectangle.getSize().y/2.f;
        loginRectangle.setPosition({ (window.getSize().x / 2.f) - radiusXLogin,
            (window.getSize().y / 2.f) - radiusYLogin });


        sf::RectangleShape usernameRectangle = createRectangle({ 200.f, 50.f }, 2.f);
        usernameRectangle.setOutlineColor(sf::Color::Black);
        usernameRectangle.setFillColor(sf::Color::White);

        float radiusXUsername = usernameRectangle.getSize().x / 2.f;
        float radiusYUsername = usernameRectangle.getSize().y / 2.f;


        usernameRectangle.setPosition({ (window.getSize().x / 2.f) - radiusXUsername,
            (window.getSize().y / 2.f) - radiusYUsername});
  
        window.clear(sf::Color::White);
        window.draw(loginRectangle);
        window.draw(usernameRectangle);
        window.display();
    }
}
