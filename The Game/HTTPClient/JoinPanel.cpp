#include "JoinPanel.h"

tgui::Panel::Ptr JoinPanel::Create(tgui::Gui& gui,
	ApiClient& apiClient,
	std::function<void()> onBack,
	std::function<void(ApiClient&, std::string)> onJoinLobby)
{
	auto background = tgui::Picture::create("assets/images/background.png");
	background->setSize({ "100%", "100%" });

	auto panel = tgui::Panel::create({ "30%", "25%" });
	panel->setPosition({ "50%", "50%" });
	panel->setOrigin(0.5f, 0.5f);
	panel->getRenderer()->setBorders({ 2 });
	panel->getRenderer()->setBorderColor(tgui::Color::Black);
	panel->getRenderer()->setRoundedBorderRadius(15);

	auto lobbyEditBox = tgui::EditBox::create();
	lobbyEditBox->setSize({ "45%", "25%" });
	lobbyEditBox->setPosition({"50%", "40%"});
	lobbyEditBox->setDefaultText("Enter lobby ID");
	lobbyEditBox->setOrigin(0.5f, 0.5f);
	lobbyEditBox->setTextSize(26);
	lobbyEditBox->onFocus([panel, lobbyEditBox]() {
		lobbyEditBox->setDefaultText("");
		});
	lobbyEditBox->onUnfocus([panel, lobbyEditBox]()
		{
			lobbyEditBox->setDefaultText("Enter lobby ID");
		});

	auto joinLobbyButton = tgui::Button::create("Join");
	joinLobbyButton->setSize({ "35%", "20%" });
	joinLobbyButton->setPosition({"60%", "70%"});
	joinLobbyButton->getRenderer()->setTexture("assets/images/buttons.png");
	joinLobbyButton->setTextSize(22);
	joinLobbyButton->onPress([onJoinLobby, &apiClient, lobbyEditBox]()
		{
			if (onJoinLobby)
			{
				onJoinLobby(apiClient, lobbyEditBox->getText().toStdString());
			}
		});

	auto backButton = tgui::Button::create("Back");
	backButton->setSize({ "35%", "20%" });
	backButton->setPosition({ "10%", "70%" });
	backButton->getRenderer()->setTexture("assets/images/buttons.png");
	backButton->setTextSize(22);
	backButton->onPress([panel, onBack]()
		{
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