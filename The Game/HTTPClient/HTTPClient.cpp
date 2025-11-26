#include <iostream>
#include <string>

#include "framework.h"
#include "LoginWindow.h"
#include "RegisterWindow.h"
#include "SFML/Graphics.hpp"
#include <cpr/cpr.h>
#include <crow.h>

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

    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/users" });
    if (!response.error)
    {
        sf::VideoMode loginRegisterMode = sf::VideoMode::getDesktopMode();
        loginRegisterMode.size = { LOGIN_WIDTH, LOGIN_HEIGHT };

        sf::RenderWindow loginWindow{ loginRegisterMode, "TheGame - Login", sf::Style::Titlebar | sf::Style::Close };
        LoginWindow _login(loginWindow);
        RegisterWindow _register(loginWindow);
        bool onLoginWindow = true;
        loginWindow.setFramerateLimit(60);

        while (loginWindow.isOpen())
        {
            std::optional<sf::Event> eventOpt;
            while ((eventOpt = loginWindow.pollEvent()))
            {
                sf::Event& event = *eventOpt;

                if (_login.GetWindowState())
                {
                    _login.HandleEvent(event);
                }
                else
                {
                    _register.HandleEvent(event);
                }

                if (event.is<sf::Event::Closed>() || _login.GetLoggedState())
                {
                    loginWindow.close();
                }
            }

            loginWindow.clear(tgui::Color::White);
            if (_login.GetWindowState())
            {
                _login.Draw();
            }
            else
            {
                _register.Draw();
            }
            loginWindow.display();
        }

        if (_login.GetLoggedState())
        {
            sf::VideoMode menuMode = sf::VideoMode::getDesktopMode();
            sf::RenderWindow menuWindow{ menuMode, "TheGame - 1.0.0", sf::State::Windowed};
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
    }

    
    return 0;
}
