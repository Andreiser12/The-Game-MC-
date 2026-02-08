#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <stdexcept>
#include <optional>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <TGUI/TGUI.hpp>
#include <cpr/cpr.h>

import ApiClient;
import LoginWindow;
import RegisterWindow;
import MainMenuWindow;
import ConnectionFailedWindow;
import HostPanel; 
import Utils;

static inline constexpr uint16_t LOGIN_WIDTH{ 640u };
static inline constexpr uint16_t LOGIN_HEIGHT{ 480u };
static inline const std::string SERVER_IP{ "192.168.1.131" };
static inline constexpr uint16_t SERVER_PORT { 18080 };
static inline const std::string SERVER_IP_ADDRESS{ "http://" + SERVER_IP + ":" + std::to_string(SERVER_PORT) };

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	sf::Image windowIcon;
	if (!windowIcon.loadFromFile("assets/images/TheGame.png")) throw std::runtime_error("TheGame.png file is missing");

	ApiClient api{ "http://localhost:18080"};
	cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/users"}, cpr::Timeout(3000));
	if (response.error || response.status_code == 404 || response.primary_port != 18080)
	{
		sf::VideoMode connectionFailedMode = sf::VideoMode::getDesktopMode();
		connectionFailedMode.size = { LOGIN_WIDTH, LOGIN_HEIGHT };
		sf::RenderWindow connectionFailedWindow{ connectionFailedMode, 
			"The Game - Failed Connection", sf::Style::Titlebar | sf::Style::Close };
		connectionFailedWindow.setIcon(
			windowIcon.getSize(),
			windowIcon.getPixelsPtr()
		);
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
	loginWindow.setIcon(
		windowIcon.getSize(),
		windowIcon.getPixelsPtr()
	);
	LoginWindow _login(loginWindow, api);
	RegisterWindow _register(loginWindow, api);

	std::thread backgroundLoader([]() {
		HostPanel::LoadGifDataAsync();
		});

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

			if (event.is<sf::Event::Closed>() || _login.GetLoggedState()
				|| _register.GetRegisteredState())
			{
				loginWindow.close();
			}
		}

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

	if (backgroundLoader.joinable()) backgroundLoader.join();

	if (_login.GetLoggedState() || _register.GetRegisteredState())
	{
		sf::VideoMode menuMode = sf::VideoMode::getDesktopMode();
		sf::RenderWindow menuWindow{ menuMode, "TheGame - 1.0.0", sf::State::Fullscreen };
		menuWindow.setIcon(
			windowIcon.getSize(),
			windowIcon.getPixelsPtr()
		);
		MainMenuWindow _menu(menuWindow, api);

		sf::Clock animationClock;

		while (menuWindow.isOpen())
		{
			float deltaTime{ animationClock.restart().asSeconds() };

			std::optional<sf::Event> eventOpt;
			while ((eventOpt = menuWindow.pollEvent()))
			{
				sf::Event& event = *eventOpt;

				_menu.HandleEvent(event);

				if (event.is<sf::Event::Closed>())
				{
					api.Logout();
					menuWindow.close();
				}
			}

			if (HostPanel::GetPlayersListBox())
			{
				HostPanel::RefreshPlayersList(api);
				HostPanel::UpdateStartButton();
				HostPanel::UpdateBackground(deltaTime);
			}
			_menu.Update();
			_menu.Draw();
			menuWindow.display();
		}
	}
	return 0;
}