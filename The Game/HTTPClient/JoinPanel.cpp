module;
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <functional>
#include <string>
#include <atomic>
#include <TGUI/TGUI.hpp>
#include <SFML/Audio.hpp>

module JoinPanel;

import ApiClient;
import HostPanel;
import Utils;

tgui::Panel::Ptr JoinPanel::Create(tgui::Gui& gui,
	ApiClient& apiClient,
	sf::Sound& btnSound,
	std::function<void()> onBack,
	std::function<bool(ApiClient&, std::string)> onJoinLobby)
{
	auto background = tgui::Picture::create("assets/images/mainMenuBackground.png");
	background->setSize("100%", "100%");

	auto panel = tgui::Panel::create({ "30%", "25%" });
	panel->setPosition("50%", "50%");
	panel->setOrigin(0.5f, 0.5f);
	panel->getRenderer()->setBorders(2);
	panel->getRenderer()->setBorderColor(tgui::Color::Black);

	auto lobbyEditBox = tgui::EditBox::create();
	lobbyEditBox->setSize("45%", "20%");
	lobbyEditBox->setPosition("50%", "42%");
	lobbyEditBox->getRenderer()->setTexture("assets/images/textBoxbackground.png");
	lobbyEditBox->getRenderer()->setFont("assets/fonts/Sanchez-Regular.ttf");
	lobbyEditBox->setDefaultText("Enter lobby ID");
	lobbyEditBox->setOrigin(0.5f, 0.5f);
	lobbyEditBox->getRenderer()->setTextSize(26);
	lobbyEditBox->setMaximumCharacters(6);

	lobbyEditBox->onFocus([lobbyEditBox]() {
		lobbyEditBox->setDefaultText("");
		});
	lobbyEditBox->onUnfocus([lobbyEditBox]()
		{
			lobbyEditBox->setDefaultText("Enter lobby ID");
		});

	auto joinLobbyButton = tgui::Button::create("Join");
	joinLobbyButton->getRenderer()->setTexture("assets/images/buttons.png");
	joinLobbyButton->setSize("35%", "20%");
	joinLobbyButton->setPosition("60%", "70%");
	Utils::StyleButton(joinLobbyButton);
	joinLobbyButton->getRenderer()->setTextSize(22);
	joinLobbyButton->onPress([&gui, onJoinLobby, &apiClient, &btnSound, lobbyEditBox, panel]()
		{
			btnSound.play();
			const std::string lobbyId{ lobbyEditBox->getText().toStdString() };

			if (lobbyId.empty())
			{
				gui.add(Utils::CreateErrorPopupPanel(gui, "Lobby ID can't be empty.", btnSound));
				return;
			}
			if (lobbyId.size() != 6)
			{
				gui.add(Utils::CreateErrorPopupPanel(gui, "Lobby ID must be 6 characters long.", btnSound));
				return;
			}

			JoinLobbyResult joinLobbyResult = apiClient.JoinLobbyValidated(lobbyId,
				apiClient.GetUsername());

			switch (joinLobbyResult)
			{
			case JoinLobbyResult::Success:
				if (onJoinLobby) onJoinLobby(apiClient, lobbyId);
				panel->setVisible(false);
				break;

			case JoinLobbyResult::LobbyNotFound:
				gui.add(Utils::CreateErrorPopupPanel(gui,
					"Lobby not found. Check the ID and try again.", btnSound));
				break;

			case JoinLobbyResult::LobbyFull:
				gui.add(Utils::CreateErrorPopupPanel(gui,
					"Lobby is full. Try another lobby", btnSound));
				break;

			case JoinLobbyResult::GameAlreadyStarted:
				gui.add(Utils::CreateErrorPopupPanel(gui,
					"Game has already started. You can't join now.", btnSound));
				break;

			case JoinLobbyResult::NetworkError:
				gui.add(Utils::CreateErrorPopupPanel(gui,
					"Network error. Please try again.", btnSound));
				break;
			}
		});

	auto backButton = tgui::Button::create("Back");
	backButton->setSize("35%", "20%");
	backButton->setPosition("10%", "70%");
	backButton->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(backButton);
	backButton->getRenderer()->setTextSize(22);
	backButton->onPress([&apiClient, &btnSound, panel, onBack]()
		{
			btnSound.play();
			panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
			if (onBack) onBack();
		});

	panel->add(background);
	panel->add(lobbyEditBox);
	panel->add(backButton);
	panel->add(joinLobbyButton);
	return panel;
}
