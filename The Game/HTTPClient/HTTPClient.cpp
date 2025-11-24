#include <iostream>
#include <string>

#include "framework.h"
#include "LoginWindow.h"
#include "SFML/Graphics.hpp"

#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#define MAX_LOADSTRING 100

constexpr static int LOGIN_HEIGHT{ 480 };
constexpr static int LOGIN_WIDTH{ 640 };

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

    sf::RenderWindow window{ loginMode, "TheGame - Login", sf::Style::Titlebar | sf::Style::Close };
    LoginWindow loginWindow(window);
    window.setFramerateLimit(60);
    while (window.isOpen())
    {
        std::optional<sf::Event> eventOpt;
        while ((eventOpt = window.pollEvent()))
        {
            sf::Event& event = *eventOpt; 

            loginWindow.HandleEvent(event);

            if (event.is<sf::Event::Closed>() || loginWindow.GetLoggedState())
            {
                window.close();
            }    
        }

        window.clear(sf::Color::White);
        loginWindow.Draw();
        window.display();
    }

    if (loginWindow.GetLoggedState())
    {
        sf::VideoMode menuMode = sf::VideoMode::getDesktopMode();
        sf::RenderWindow menuWindow{ menuMode, "TheGame - 1.0.0", sf::State::Fullscreen};
        while (menuWindow.isOpen())
        {
            std::optional<sf::Event> eventOpt;
            while ((eventOpt = menuWindow.pollEvent()))
            {
                sf::Event& event = *eventOpt;

                if (event.is<sf::Event::Closed>())
                {
                    menuWindow.close();
                }
            }
            menuWindow.clear(sf::Color::White);
            menuWindow.display();
        }
    }
    return 0;
}
