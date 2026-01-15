#include "JoinPanel.h"

std::atomic<bool> JoinPanel::s_running{ false };

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
			std::string errorMessage;

			if (lobbyId.empty()) errorMessage = "Lobby ID can't be empty";
			else if (lobbyId.size() != 6) errorMessage = "Lobby must be 6 characters long";

			if (!errorMessage.empty())
			{
				auto errorPopupPanel = Utils::CreateErrorPopupPanel(gui, errorMessage, btnSound);
				gui.add(errorPopupPanel);
				return;
			}

			JoinLobbyResult joinLobbyResult = apiClient.JoinLobbyValidated(lobbyId,
				apiClient.GetUsername());

			switch (joinLobbyResult)
			{
			case JoinLobbyResult::Success:
				if (onJoinLobby) onJoinLobby(apiClient, lobbyId);
				panel->setVisible(false);
				apiClient.StartPingThread(lobbyId, apiClient.GetPlayerId());
				s_running.store(true);
				break;

			case JoinLobbyResult::LobbyNotFound:
				errorMessage = "Lobby not found. Check the ID and try again.";
				break;

			case JoinLobbyResult::LobbyFull:
				errorMessage = "Lobby is full. Try another lobby.";
				break;
				
			case JoinLobbyResult::GameAlreadyStarted:
				errorMessage = "Game has already started. You can't join now.";
				break;

			case JoinLobbyResult::NetworkError:
				errorMessage = "Network error. Please try again.";
				break;
			}

			if (!errorMessage.empty())
			{
				auto errorPopupPanel = Utils::CreateErrorPopupPanel(gui, errorMessage, btnSound);
				gui.add(errorPopupPanel);
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
			apiClient.StopPingThread();
			panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
			if (onBack)
			{
				onBack();
			}
		});

	panel->add(background);
	panel->add(lobbyEditBox);
	panel->add(backButton);
	panel->add(joinLobbyButton);
	return panel;
}
void JoinPanel::StopUpdateThread()
{
	s_running.store(false);
}
