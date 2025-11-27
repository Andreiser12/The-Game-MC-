#include <iostream>
#include <string>
#include "framework.h"

#include "LoginWindow.h"
#include "RegisterWindow.h"
#include "MainMenuWindow.h"
#include "ConnectionFailedWindow.h"
#include "SFML/Graphics.hpp"
#include <cpr/cpr.h>
#include <crow.h>

constexpr static int LOGIN_WIDTH{ 640 };
constexpr static int LOGIN_HEIGHT{ 480 };

HINSTANCE hInst;

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
	if ((int)response.status_code == 404 || response.primary_port!=18080)
	{
		sf::VideoMode connectionFailedMode = sf::VideoMode::getDesktopMode();
		connectionFailedMode.size = { LOGIN_WIDTH, LOGIN_HEIGHT };
		sf::RenderWindow connectionFailedWindow{ connectionFailedMode, "Connection Failed", sf::Style::Titlebar | sf::Style::Close };
		ConnectionFailedWindow _connection(connectionFailedWindow);

		while (connectionFailedWindow.isOpen())
		{
			std::optional<sf::Event> eventOpt;
			while ((eventOpt = connectionFailedWindow.pollEvent()))
			{
				sf::Event& event = *eventOpt;


				if (event.is<sf::Event::Closed>())
				{
					connectionFailedWindow.close();
				}
			}

			connectionFailedWindow.clear(tgui::Color::Black);
			_connection.Draw();
			connectionFailedWindow.display();
		}
		return 1;
	}
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

			if (event.is<sf::Event::Closed>() || _login.GetLoggedState() || _register.GetRegisteredState())
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

	if (_login.GetLoggedState() || _register.GetRegisteredState())
	{
		sf::VideoMode menuMode = sf::VideoMode::getDesktopMode();
		sf::RenderWindow menuWindow{ menuMode, "TheGame - 1.0.0", sf::State::Fullscreen };
		MainMenuWindow _menu(menuWindow);
		while (menuWindow.isOpen())
		{
			std::optional<sf::Event> eventOpt;
			while ((eventOpt = menuWindow.pollEvent()))
			{
				sf::Event& event = *eventOpt;

				_menu.HandleEvent(event);

				if (event.is<sf::Event::Closed>())
				{
					menuWindow.close();
				}
			}
			menuWindow.clear(tgui::Color(180, 125, 23));
			_menu.Draw();
			menuWindow.display();
		}
	}

	return 0;
}
