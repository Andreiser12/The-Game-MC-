#pragma once
#include "IWindow.h"
#include "ApiClient.h"
#include "Utils.h"

class RegisterWindow : public IWindow
{
public:
	RegisterWindow(sf::RenderWindow& window, ApiClient& api);

	void HandleEvent(sf::Event event) override;
	void Draw() override;
	void GoToLogin();
	
	void CheckRegister(const std::string& usernameEntered);
	bool GetRegisteredState() const;

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

