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
			throw std::runtime_error("One of the GIF file is missing");
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
	lobbyIdLabel->setPosition("70%", "0%");
	lobbyIdLabel->getRenderer()->setFont("assets/fonts/Sanchez-Regular.ttf");
	lobbyIdLabel->getRenderer()->setBorders(1);
	lobbyIdLabel->getRenderer()->setBorderColor(tgui::Color::Black);
	lobbyIdLabel->getRenderer()->setBackgroundColor(tgui::Color(128, 128, 128));
	lobbyIdLabel->getRenderer()->setOpacity(0.7f);
	lobbyIdLabel->getRenderer()->setTextSize(20);

	playersListBox = tgui::ListBox::create();
	playersListBox->getRenderer()->setFont("assets/fonts/Sanchez-Regular.ttf");
	playersListBox->setSize("20%", "30%");
	playersListBox->setPosition("0%", "0%");
	playersListBox->getRenderer()->setOpacity(0.7f);
	playersListBox->getRenderer()->setTextSize(20);
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

	apiClient.StartNetworkLoop();
	auto leaveButton = tgui::Button::create("Leave Lobby");
	leaveButton->setSize("22.5%", "12.5%");
	leaveButton->setPosition("3.5%", "82%");
	leaveButton->getRenderer()->setFont("assets/fonts/Sanchez-Regular.ttf");
	leaveButton->getRenderer()->setTextSize(20);
	leaveButton->getRenderer()->setTextColor(tgui::Color::White);
	leaveButton->getRenderer()->setBackgroundColor(tgui::Color(90, 90, 90));
	leaveButton->getRenderer()->setBackgroundColorHover(tgui::Color(120, 120, 120));
	leaveButton->getRenderer()->setBackgroundColorDown(tgui::Color(70, 70, 70));
	leaveButton->getRenderer()->setBorderColor(tgui::Color(40, 40, 40));
	leaveButton->getRenderer()->setBorders(1);
	leaveButton->getRenderer()->setOpacity(0.85f);
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
	startButtonLocal->setPosition("74%", "82%");
	startButtonLocal->getRenderer()->setFont("assets/fonts/Sanchez-Regular.ttf");
	startButtonLocal->getRenderer()->setTextSize(20);
	startButtonLocal->getRenderer()->setTextColor(tgui::Color::White);
	startButtonLocal->getRenderer()->setBackgroundColor(tgui::Color(60, 120, 90));
	startButtonLocal->getRenderer()->setBackgroundColorHover(tgui::Color(80, 150, 115));
	startButtonLocal->getRenderer()->setBackgroundColorDown(tgui::Color(45, 95, 70));
	startButtonLocal->getRenderer()->setBorderColor(tgui::Color(30, 60, 50));
	startButtonLocal->getRenderer()->setBackgroundColorDisabled(tgui::Color(60, 80, 75));
	startButtonLocal->getRenderer()->setTextColorDisabled(tgui::Color(170, 170, 170));
	startButtonLocal->getRenderer()->setBorderColorDisabled(tgui::Color(50, 70, 65));
	startButtonLocal->getRenderer()->setBorders(1);
	startButtonLocal->getRenderer()->setOpacity(0.85f);
	startButton = startButtonLocal;

	bool isHost{ (s_playerId != -1 && s_hostId != -1 && s_playerId == s_hostId) };
	bool enoughPlayers{ playersListBox->getItemCount() - 1 >= kMinPlayers };
	startButton->setVisible(isHost);
	startButton->setEnabled(isHost && enoughPlayers);
	startButton->onPress([onStart, &apiClient, &btnSound]()
		{
			btnSound.play();
			if (onStart) onStart(apiClient);
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
		playersListBox->addItem("Players:");
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
	bool enoughPlayers{ (playersListBox->getItemCount() - 1 >= kMinPlayers) };
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