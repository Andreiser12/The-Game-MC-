#include "JoinPanel.h"

tgui::Panel::Ptr JoinPanel::Create(tgui::Gui& gui,
	std::function<void()> onBack,
	std::function<void()> onJoinLobby)
{
	auto background = tgui::Picture::create("assets/images/background.png");
	background->setSize({ "100%", "100%" });

	auto panel = tgui::Panel::create({ "30%", "25%" });
	panel->setPosition({ "50%", "50%" });
	panel->setOrigin(0.5f, 0.5f);
	panel->getRenderer()->setBorders({ 2 });
	panel->getRenderer()->setBorderColor(tgui::Color::Black);
	panel->getRenderer()->setRoundedBorderRadius(15);

	auto lobbyBox = tgui::EditBox::create();
	lobbyBox->setSize({ "45%", "25%" });
	lobbyBox->setPosition({"50%", "40%"});
	lobbyBox->setDefaultText("Enter lobby ID");
	lobbyBox->setOrigin(0.5f, 0.5f);
	lobbyBox->setTextSize(26);
	lobbyBox->onFocus([panel, lobbyBox]() {
		lobbyBox->setDefaultText("");
		});
	lobbyBox->onUnfocus([panel, lobbyBox]()
		{
			lobbyBox->setDefaultText("Enter lobby ID");
		});

	auto joinLobbyButton = tgui::Button::create("Join");
	joinLobbyButton->setSize({ "35%", "20%" });
	joinLobbyButton->setPosition({"60%", "70%"});
	joinLobbyButton->getRenderer()->setTexture("assets/images/buttons.png");
	joinLobbyButton->setTextSize(22);
	joinLobbyButton->onPress([onJoinLobby, lobbyBox]()
		{
			if (onJoinLobby)
			{
				onJoinLobby();
			}
		});

	auto backButton = tgui::Button::create("Back");
	backButton->setSize({ "35%", "20%" });
	backButton->setPosition({ "10%", "70%" });
	backButton->getRenderer()->setTexture("assets/images/buttons.png");
	backButton->setTextSize(22);
	backButton->onPress([onBack, panel]()
		{
			panel->hideWithEffect(tgui::ShowEffectType::Fade, sf::milliseconds(150));
			if (onBack)
			{
				onBack();
			}
		});

	panel->add(background);
	panel->add(lobbyBox);
	panel->add(backButton);
	panel->add(joinLobbyButton);

	return panel;
}