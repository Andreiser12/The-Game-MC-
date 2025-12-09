#include "HostPanel.h"

tgui::Panel::Ptr HostPanel::Create(tgui::Gui& gui,
	ApiClient& apiClient,
	const std::string& username,
	const std::string& lobbyId,
	std::function<bool(ApiClient&)> onLeave,
	std::function<void()> onStart)
{
	auto background = tgui::Picture::create("assets/images/background.png");
	background->setSize({ "100%", "100%" });

	auto panel = tgui::Panel::create({ "40%", "40%" });
	panel->setPosition({ "50%", "50%" });
	panel->setOrigin(0.5f, 0.5f);
	panel->getRenderer()->setBorders({ 2 });
	panel->getRenderer()->setBorderColor(tgui::Color::Black);
	panel->getRenderer()->setRoundedBorderRadius(15);

	auto lobbyIdLabel = tgui::Label::create("Lobby ID: " + lobbyId);
	lobbyIdLabel->setSize({ "30%", "10%" });
	lobbyIdLabel->setPosition({"62.5%", "7.5%"});
	lobbyIdLabel->getRenderer()->setBorders({ 1 });
	lobbyIdLabel->getRenderer()->setBorderColor(tgui::Color::Black);
	lobbyIdLabel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	lobbyIdLabel->getRenderer()->setOpacity(0.7f);
	lobbyIdLabel->setTextSize(22);

	auto usernameLabel = tgui::Label::create(username);
	usernameLabel->setSize({ "25%", "8%" });
	usernameLabel->setPosition({ "25%", "7.5%" });
	usernameLabel->getRenderer()->setBorders({ 1 });
	usernameLabel->getRenderer()->setBorderColor(tgui::Color::Black);
	usernameLabel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	usernameLabel->getRenderer()->setOpacity(0.7f);
	usernameLabel->setTextSize(24);
	
	auto leaveButton = tgui::Button::create("Leave Lobby");
	leaveButton->setSize({ "22.5%", "12.5%" });
	leaveButton->setPosition({ "27.5%", "80%" });
	leaveButton->getRenderer()->setTexture("assets/images/buttons.png");
	leaveButton->setTextSize(18);
	leaveButton->onPress([panel, &apiClient, onLeave]()
		{
			if (onLeave)
			{
				if (onLeave(apiClient))
				{
					panel->setVisible(false);
				}
			}
		});

	auto startButton = tgui::Button::create("Start Match");
	startButton->setSize({ "22.5%", "12.5%" });
	startButton->setPosition({"65%", "80%"});
	startButton->getRenderer()->setTexture("assets/images/buttons.png");
	startButton->setTextSize(18);
	startButton->onPress([]()
	{

	});

	panel->add(background);
	panel->add(leaveButton);
	panel->add(startButton);
	panel->add(lobbyIdLabel);
	panel->add(usernameLabel);

	return panel;
}
