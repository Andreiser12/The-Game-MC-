module;
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

export module LoginWindow;

import IWindow;
import ApiClient;
import Utils;

export class LoginWindow : public IWindow
{
public:
	explicit LoginWindow(sf::RenderWindow& window, ApiClient& api);

	LoginWindow(const LoginWindow&) = delete;
	LoginWindow& operator=(const LoginWindow&) = delete;
	LoginWindow(LoginWindow&&) noexcept = default;
	LoginWindow& operator=(LoginWindow&&) noexcept = default;

	void CheckLogin(const std::string& usernameEntered);

	void Draw() override;
	void HandleEvent(sf::Event event) override;

	bool GetLoggedState() const noexcept;
	bool GetRegisteredState() const noexcept;
	bool GetWindowState() const noexcept override;

private:
	void LoadWidgets() override;
	
private:

	tgui::Gui m_login;
	tgui::Picture::Ptr m_background;
	bool m_isLoggedIn{ false };
	bool m_isRegisteredPressed{ false };
	ApiClient& m_apiClient;

	sf::SoundBuffer m_clickBuffer;
	sf::Sound m_clickSound;
};

