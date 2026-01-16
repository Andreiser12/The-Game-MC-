module;
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <string>
#include <functional>
#include <SFML/Audio.hpp>
#include <TGUI/TGUI.hpp>

module PlayPanel;

import JoinPanel;
import HostPanel;
import ApiClient;
import Utils; 

tgui::Panel::Ptr PlayPanel::Create(tgui::Gui& gui,
	ApiClient& apiClient,
	sf::Sound& btnSound,
	std::function<void()> onClose,
	std::function<void()> onJoin,
	std::function<std::string(ApiClient&, const std::string&)> onHost,
	std::function<void()> onTransition)
{
	auto background = tgui::Picture::create("assets/images/mainMenuBackground.png");
	background->setSize("100%", "100%");
	background->setPosition("0%", "0%");

	auto panel = tgui::Panel::create({ "30%", "25%" });
	panel->setPosition("50%", "50%");
	panel->getRenderer()->setBorders(2);
	panel->getRenderer()->setBorderColor(tgui::Color::Black);
	panel->setOrigin(0.5f, 0.5f);

	auto titleLabel = tgui::Label::create("Select an option");
	titleLabel->setPosition("50%", "44%");
	titleLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	titleLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	titleLabel->setOrigin(0.5f, 0.5f);
	titleLabel->getRenderer()->setTextColor(tgui::Color(245, 240, 230));
	titleLabel->getRenderer()->setTextOutlineColor(tgui::Color(40, 25, 15));
	titleLabel->getRenderer()->setTextOutlineThickness(1.5f);
	titleLabel->getRenderer()->setTextSize(26);

	auto joinButton = tgui::Button::create("Join Lobby");
	joinButton->getRenderer()->setTexture("assets/images/buttons.png");
	joinButton->setSize("35%", "20%");
	joinButton->setPosition("5%", "70%");
	Utils::StyleButton(joinButton);
	joinButton->getRenderer()->setTextSize(22);
	joinButton->onPress([&gui, &apiClient, &btnSound, panel, onTransition, onJoin]() {
		btnSound.play();
		panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
		auto joinPanel = JoinPanel::Create(
			gui,
			apiClient,
			btnSound,
			[panel]() { panel->setVisible(true); },
			[&gui, &apiClient, &btnSound, panel, onTransition](ApiClient& api, std::string lobbyIdEntered)
			{
				panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
				std::string lobbyId{ api.GetLobbyId() };
				auto host = HostPanel::Create(
					gui,
					apiClient,
					btnSound,
					lobbyId,
					[lobbyId](ApiClient& api) { return api.LeaveLobby(lobbyId, api.GetPlayerId()); },
					[lobbyId, onTransition](ApiClient& api) {
						bool success{ api.StartMatch(lobbyId, api.GetPlayerId()) };
						if (success && onTransition) onTransition();
						return success;
					}
				);

				gui.add(host, "HostPanel");
				return true;
			}
		);
		gui.add(joinPanel);
		if (onJoin) onJoin();
		});

	auto hostButton = tgui::Button::create("Host Lobby");
	hostButton->setSize("35%", "20%");
	hostButton->setPosition("60%", "70%");
	hostButton->getRenderer()->setTexture("assets/images/buttons.png");
	Utils::StyleButton(hostButton);
	hostButton->getRenderer()->setTextSize(22);
	hostButton->onPress([&gui, &apiClient, &btnSound, onHost, onTransition, panel]() {
		btnSound.play();
		if (onHost)
		{
			std::string lobbyId{ onHost(apiClient, apiClient.GetUsername()) };
			apiClient.SetLobbyId(lobbyId);
			panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
			auto hostPanel = HostPanel::Create(
				gui,
				apiClient,
				btnSound,
				lobbyId,
				[lobbyId](ApiClient& api) { return api.LeaveLobby(lobbyId, api.GetPlayerId()); },
				[lobbyId, onTransition](ApiClient& api) {
					bool success = api.StartMatch(lobbyId, api.GetPlayerId());
					if (success && onTransition) onTransition();
					return success;
				}
			);
			gui.add(hostPanel, "HostPanel");
		}
		});

	auto closeButton = tgui::Button::create("X");
	closeButton->setSize("10%", "10%");
	closeButton->setPosition("90%", "0%");
	closeButton->getRenderer()->setTexture("assets/images/buttons.png");
	closeButton->getRenderer()->setBorders(1);
	Utils::StyleButton(closeButton);
	closeButton->getRenderer()->setTextSize(16);
	closeButton->onPress([panel, onClose, &btnSound]()
		{
			btnSound.play();
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