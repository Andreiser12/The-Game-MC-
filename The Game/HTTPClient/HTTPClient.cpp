// HTTPClient.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "HTTPClient.h"
#include "SFML/Graphics.hpp"

#define MAX_LOADSTRING 100

constexpr static int LOGIN_HEIGHT{ 480 };
constexpr static int LOGIN_WIDTH{ 640 };

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
        sf::RectangleShape loginRectangle({ 325.f, 140.f });
        loginRectangle.setOutlineColor(sf::Color::Black);
        loginRectangle.setFillColor(sf::Color::Blue);
        float radiusX = loginRectangle.getSize().x/2;
        float radiusY = loginRectangle.getSize().y / 2;
        loginRectangle.setPosition({ (window.getSize().x / 2.f) - radiusX, (window.getSize().y / 2.f) - radiusY });

        window.clear(sf::Color::White);
        window.draw(loginRectangle);
        window.display();
    }
}
