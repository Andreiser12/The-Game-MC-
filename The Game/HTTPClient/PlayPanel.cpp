#include "PlayPanel.h"

tgui::Panel::Ptr PlayPanel::Create(tgui::Gui& gui,
	ApiClient& apiClient,
	std::function<void()> onClose,
	std::function<void()> onJoin,
	std::function<std::string(ApiClient&, const std::string&)> onHost)
{
	auto background = tgui::Picture::create("assets/images/background.png");
	background->setSize({ "100%", "100%" });

	auto panel = tgui::Panel::create({"30%", "25%"});
	panel->setPosition({ "50%", "50%" });
	panel->getRenderer()->setBorders({ 2 });
	panel->getRenderer()->setBorderColor(tgui::Color::Black);
	panel->getRenderer()->setRoundedBorderRadius(15);
	panel->setOrigin(0.5f, 0.5f);

	auto titleLabel = tgui::Label::create("Select option");
	titleLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	titleLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	titleLabel->setPosition({ "50%", "35%" });
	titleLabel->setOrigin(0.5f, 0.5f);
	titleLabel->setTextSize(26);

	auto joinButton = tgui::Button::create("Join Lobby");
	joinButton->setTextSize(22);
	joinButton->getRenderer()->setTexture("assets/images/buttons.png");
	joinButton->setSize({ "35%", "20%" });
	joinButton->setPosition({ "5%","70%" });
	joinButton->onPress([&gui, onJoin, &apiClient, panel]() {
		panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
		auto joinPanel = JoinPanel::Create(
			gui,
			apiClient,
			[panel]() {
				panel->setVisible(true);
			},
			[](ApiClient& api, std::string lobbyIdEntered)
			{
				api.JoinLobby(lobbyIdEntered, api.GetUsername());
			}
			);
		gui.add(joinPanel);
		if (onJoin) onJoin();
		});

	auto hostButton = tgui::Button::create("Host Lobby");
	hostButton->setTextSize(22);
	hostButton->setSize({ "35%", "20%" });
	hostButton->getRenderer()->setTexture("assets/images/buttons.png");
	hostButton->setPosition({ "60%", "70%" });
	hostButton->onPress([&gui, panel, &apiClient, onHost]() {
		if (onHost)
		{
			std::string lobbyId = onHost(apiClient, apiClient.GetUsername());
			panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(120));
			auto hostPanel = HostPanel::Create(gui,
				apiClient,
				lobbyId,
				[lobbyId](ApiClient& api) { return api.LeaveLobby(lobbyId, api.GetPlayerId()); },
				[]() {}
			);
			gui.add(hostPanel);
		}
		});

	auto closeButton = tgui::Button::create("X");
	closeButton->setTextSize(22);
	closeButton->setSize({"10%", "10%"});
	closeButton->setPosition({ "90%", "0%" });
	closeButton->getRenderer()->setTexture("assets/images/buttons.png");
	closeButton->getRenderer()->setBorders({ 1 });
	closeButton->getRenderer()->setBorderColor(tgui::Color::White);
	closeButton->getRenderer()->setRoundedBorderRadius(15);
	closeButton->onPress([panel, onClose]() 
		{
			panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
			if (onClose) onClose();
		});

	panel->add(background);
	panel->add(closeButton);
	panel->add(joinButton);
	panel->add(hostButton);
	panel->add(titleLabel);

	return panel;
}

