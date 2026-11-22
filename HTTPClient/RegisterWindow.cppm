module;
#include <string>
#include <vector>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

export module RegisterWindow;

import IWindow;
import ApiClient;
import Utils;

export class RegisterWindow : public IWindow
{
public:
	explicit RegisterWindow(sf::RenderWindow& window, ApiClient& api);

	RegisterWindow(const RegisterWindow&) = delete;
	RegisterWindow& operator=(const RegisterWindow&) = delete;
	RegisterWindow(RegisterWindow&&) noexcept = default;
	RegisterWindow& operator=(RegisterWindow&&) noexcept = default;

	void HandleEvent(sf::Event event) override;
	void Draw() override;
	void GoToLogin();

	void CheckRegister(const std::string& usernameEntered);
	bool GetRegisteredState() const noexcept;

private:
	void LoadWidgets() override;

private:
	tgui::Gui m_register;
	tgui::Picture::Ptr m_background;
	tgui::Picture::Ptr m_gameTitle;
	bool m_isRegistered{ false };
	ApiClient& m_apiClient;

	sf::SoundBuffer m_clickBuffer;
	sf::Sound m_clickSound;
};

