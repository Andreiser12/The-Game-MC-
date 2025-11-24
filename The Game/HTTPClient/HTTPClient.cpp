#include <iostream>
#include <string>

#include "framework.h"
#include "LoginWindow.h"
#include "RegisterWindow.h"
#include "SFML/Graphics.hpp"

#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"

#define MAX_LOADSTRING 100

constexpr static int LOGIN_WIDTH{ 640 };
constexpr static int LOGIN_HEIGHT{ 480 };

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

    sf::RenderWindow loginWindow{ loginMode, "TheGame - Login", sf::Style::Titlebar | sf::Style::Close };
    LoginWindow login(loginWindow);
    loginWindow.setFramerateLimit(60);
    while (loginWindow.isOpen())
    {
        std::optional<sf::Event> eventOpt;
        while ((eventOpt = loginWindow.pollEvent()))
        {
            sf::Event& event = *eventOpt; 

            login.HandleEvent(event);

            if (event.is<sf::Event::Closed>() || login.GetLoggedState()
                || login.GetRegisteredState())
            {
                loginWindow.close();
            }    
        }

        loginWindow.clear(sf::Color::White);
        login.Draw();
        loginWindow.display();
    }

    if (login.GetRegisteredState())
    {
       
        sf::VideoMode registerMode = sf::VideoMode::getDesktopMode();
        registerMode.size = { LOGIN_WIDTH, LOGIN_HEIGHT };
        sf::RenderWindow registerWindow(registerMode, "TheGame - Register", sf::Style::Titlebar | sf::Style::Close);
        RegisterWindow _register(registerWindow);

        while (registerWindow.isOpen())
        {
            std::optional<sf::Event> eventOpt;
            while ((eventOpt = registerWindow.pollEvent()))
            {
                sf::Event& event = *eventOpt;

                _register.HandleEvent(event);

                if (event.is<sf::Event::Closed>() || _register.GetRegisteredState())
                {
                    registerWindow.close();
                }
            }
            registerWindow.clear(sf::Color::White);
            _register.Draw();
            registerWindow.display();
        }
    }

    if (login.GetLoggedState())
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
