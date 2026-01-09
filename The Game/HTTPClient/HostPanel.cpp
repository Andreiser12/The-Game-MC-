#include "HostPanel.h"

static constexpr uint8_t kMinPlayers{ 2u };
static std::vector<tgui::Texture> s_frames;
static tgui::Picture::Ptr s_backgroundPicture;
static float s_animationTime{ 0.0f };
static int s_currentFrame{ 0 };
static const float kFrameDuration{ 0.04f };

void HostPanel::PreloadImages()
{
	if (!s_frames.empty()) return;

	constexpr uint16_t totalFrames{ 300 };

	for (uint16_t i = 0; i < totalFrames; ++i)
	{
		std::stringstream ss;

		ss << "assets/gif/TheGameLevelSequence."
			<< std::setw(4) << std::setfill('0') << i
			<< ".jpeg";

		std::string path = ss.str();

		try {
			s_frames.emplace_back(path);
		}
		catch (...) {
			std::cout << "Nu am putut incarca: " << path << std::endl;
		}
	}
}

tgui::Panel::Ptr HostPanel::Create(tgui::Gui& gui,
	ApiClient& apiClient,
	sf::Sound& btnSound,
	const std::string& lobbyId,
	std::function<bool(ApiClient&)> onLeave,
	std::function<bool(ApiClient&)> onStart)
{
	PreloadImages();

	if (!s_frames.empty())
	{
		s_backgroundPicture = tgui::Picture::create(s_frames[0]);
	}
	else
	{
		s_backgroundPicture = tgui::Picture::create("assets/images/mainMenuBackground.png");
	}
	s_backgroundPicture->setSize("100%", "100%");

	auto panel = tgui::Panel::create({ "40%", "40%" });
	panel->setPosition("50%", "50%");
	panel->setOrigin(0.5f, 0.5f);
	panel->getRenderer()->setBorders(2);
	panel->getRenderer()->setBorderColor(tgui::Color::Black);
	panel->setWidgetName("HostPanel");

	auto lobbyIdLabel = tgui::Label::create("Lobby ID: " + lobbyId);
	lobbyIdLabel->setSize("30%", "10%");
	lobbyIdLabel->setPosition("62.5%", "7.5%");
	lobbyIdLabel->getRenderer()->setFont("assets/fonts/Sanchez-Regular.ttf");
	lobbyIdLabel->getRenderer()->setBorders(1);
	lobbyIdLabel->getRenderer()->setBorderColor(tgui::Color::Black);
	lobbyIdLabel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	lobbyIdLabel->getRenderer()->setOpacity(0.7f);
	lobbyIdLabel->getRenderer()->setTextSize(22);

	playersListBox = tgui::ListBox::create();
	playersListBox->setSize("20%", "30%");
	playersListBox->setPosition("0%", "5%");
	playersListBox->getRenderer()->setFont("assets/fonts/Sanchez-Regular.ttf");
	playersListBox->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	for (const auto& username : apiClient.SnapshotPlayers())
	{
		playersListBox->addItem(username);
	}

	if (auto lobbyState = apiClient.GetLobbyState(lobbyId))
	{
		s_hostId = lobbyState->m_hostId;
		s_playerId = apiClient.GetPlayerId();
	}

	auto usernameLabel = tgui::Label::create(apiClient.GetUsername());
	usernameLabel->setSize("25%", "8%");
	usernameLabel->setPosition("25%", "7.5%");
	usernameLabel->getRenderer()->setBorders(1);
	usernameLabel->getRenderer()->setBorderColor(tgui::Color::Black);
	usernameLabel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	usernameLabel->getRenderer()->setOpacity(0.7f);
	usernameLabel->getRenderer()->setTextSize(24);

	apiClient.StartNetworkLoop();
	auto leaveButton = tgui::Button::create("Leave Lobby");
	leaveButton->setSize("22.5%", "12.5%");
	leaveButton->setPosition("27.5%", "80%");
	leaveButton->getRenderer()->setTexture("assets/images/buttons.png");
	leaveButton->getRenderer()->setTextSize(18);
	leaveButton->onPress([&apiClient, &btnSound, onLeave, panel]()
		{
			btnSound.play();
			apiClient.StopNetworkLoop();
			playersListBox = nullptr;
			startButton = nullptr;
			if (onLeave && onLeave(apiClient)) panel->setVisible(false);
		});

	auto startButtonLocal = tgui::Button::create("Start Match");
	startButtonLocal->setSize("22.5%", "12.5%");
	startButtonLocal->setPosition("65%", "80%");
	startButtonLocal->getRenderer()->setTexture("assets/images/buttons.png");
	startButtonLocal->getRenderer()->setTextSize(18);
	startButton = startButtonLocal;

	bool isHost{ (s_playerId != -1 && s_hostId != -1 && s_playerId == s_hostId) };
	bool enoughPlayers{ playersListBox->getItemCount() >= kMinPlayers };
	startButton->setVisible(isHost);
	startButton->setEnabled(isHost && enoughPlayers);
	startButton->onPress([onStart, &apiClient, &btnSound]()
		{
			btnSound.play();
			if (onStart)
			{
				onStart(apiClient);
			}
		});

	panel->add(leaveButton);
	panel->add(startButton);
	panel->add(lobbyIdLabel);
	panel->add(playersListBox);
	panel->add(s_backgroundPicture);
	s_backgroundPicture->moveToBack();
	return panel;
}

void HostPanel::RefreshPlayersList(ApiClient& apiClient)
{
	static std::vector<std::string> lastPlayers;
	auto playersCopy = apiClient.SnapshotPlayers();

	if (lastPlayers == playersCopy) return;

	lastPlayers = playersCopy;
	if (playersListBox)
	{
		playersListBox->removeAllItems();
		for (const auto& username : playersCopy)
		{
			playersListBox->addItem(username);
		}
	}
}

void HostPanel::UpdateStartButton()
{
	if (!startButton || !playersListBox) return;

	bool isHost{ (s_hostId == s_playerId) };
	bool enoughPlayers{ (playersListBox->getItemCount() >= kMinPlayers) };
	startButton->setVisible(isHost);
	startButton->setEnabled(isHost && enoughPlayers);
}

void HostPanel::UpdateBackground(float deltaTime)
{
	if (s_frames.empty() || !s_backgroundPicture) return;

	s_animationTime += deltaTime;

	while (s_animationTime >= kFrameDuration)
	{
		s_animationTime -= kFrameDuration;
		s_currentFrame = (s_currentFrame + 1) % s_frames.size();
	}

	s_backgroundPicture->getRenderer()->setTexture(s_frames[s_currentFrame]);
}
