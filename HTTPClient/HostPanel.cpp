module;
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_GIF

#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <SFML/Audio.hpp>
#include "stb_image.h"
#include <mutex>
#include <vector>
#include <functional>
#include <stdexcept>
#include <fstream>

module HostPanel;

static constexpr uint8_t kMinPlayers{ 2u };
static const float kFrameDuration{ 0.04f };
static const int kTextureLimit{ 1 };
static inline int s_hostId{ -1 };
static inline int s_playerId{ -1 };

namespace HostPanel
{
	tgui::ListBox::Ptr playersListBox{ nullptr };
	tgui::Button::Ptr startButton{ nullptr };
	tgui::Picture::Ptr s_backgroundPicture{ nullptr };

	float s_animationTime{ 0.0f };
	int s_currentFrame{ 0 };

	std::vector<RawFrameData> s_rawFrames;
	std::vector<tgui::Texture> s_frames;
	std::mutex s_framesMutex;

	std::vector<uint8_t> ReadFileToBuffer(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file) return {};

		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8_t> buffer(size);
		if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
			return buffer;
		}
		return {};
	}

	bool ProcessNextRawFrame()
	{
		return false;
	}

	tgui::Panel::Ptr HostPanel::Create(tgui::Gui& gui,
		ApiClient& apiClient,
		sf::Sound& btnSound,
		const std::string& lobbyId,
		std::function<bool(ApiClient&)>&& onLeave,
		std::function<bool(ApiClient&)>&& onStart)
	{
		if (s_frames.empty() && !s_rawFrames.empty())
		{
			GenerateTextures(kTextureLimit);
		}

		if (!s_frames.empty())
		{
			s_backgroundPicture = tgui::Picture::create(s_frames[0]);
		}
		else
		{
			s_backgroundPicture = tgui::Picture::create("assets/images/hostPanelBackground.png");
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
		playersListBox->setSize({ "20%", "30%" });
		playersListBox->setPosition("0%", "0%");
		playersListBox->getRenderer()->setTextSize(20);
		playersListBox->getRenderer()->setBackgroundColor(tgui::Color(30, 35, 45));
		playersListBox->getRenderer()->setTextColor(tgui::Color(220, 220, 230));
		playersListBox->getRenderer()->setTextColorHover(tgui::Color(255, 255, 255));
		playersListBox->getRenderer()->setBackgroundColorHover(tgui::Color(50, 60, 80));
		playersListBox->getRenderer()->setSelectedBackgroundColor(tgui::Color(70, 130, 180));
		playersListBox->getRenderer()->setSelectedTextColor(tgui::Color(255, 255, 255));

		playersListBox->getRenderer()->setBorderColor(tgui::Color(70, 130, 180));
		playersListBox->getRenderer()->setBorders(2);
		playersListBox->getRenderer()->setOpacity(0.7f);
		playersListBox->getRenderer()->setPadding({ 10, 5, 10, 5 });

		for (const auto& username : apiClient.SnapshotPlayers())
		{
			playersListBox->addItem(username);
		}

		auto info = apiClient.GetLobbyInfo(lobbyId);
		if (info)
		{
			if (info->has("host_id"))
			{
				s_hostId = static_cast<unsigned>((*info)["host_id"].i());
			}
		}
		s_playerId = apiClient.GetPlayerId();

		apiClient.StartNetworkLoop();
		auto leaveButton = tgui::Button::create("Leave Lobby");
		leaveButton->setSize({ "22.5%", "12.5%" });
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
		startButtonLocal->setSize({ "22.5%", "12.5%" });
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

		bool isHost{ (s_playerId != 0 && s_hostId != 0 && s_playerId == s_hostId) };
		bool enoughPlayers{ apiClient.SnapshotPlayers().size() >= kMinPlayers };
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

	tgui::ListBox::Ptr GetPlayersListBox()
	{
		return playersListBox;
	}

	void HostPanel::UpdateBackground(float deltaTime)
	{
		GenerateTextures(kTextureLimit);
		if (s_frames.empty() || !s_backgroundPicture) return;

		s_animationTime += deltaTime;

		while (s_animationTime >= kFrameDuration)
		{
			s_animationTime -= kFrameDuration;
			s_currentFrame = (s_currentFrame + 1) % s_frames.size();
		}
		s_backgroundPicture->getRenderer()->setTexture(s_frames[s_currentFrame]);
	}

	void HostPanel::LoadGifDataAsync()
	{
		{
			std::lock_guard<std::mutex> lock(s_framesMutex);
			if (!s_frames.empty() || !s_rawFrames.empty()) return;
		}

		std::vector<uint8_t> fileBuffer = ReadFileToBuffer("assets/gif/gif_animation.gif");
		if (fileBuffer.empty()) return;

		int* delays{ nullptr };
		int x{ 0 }, y{ 0 }, frames{ 0 }, comp{ 0 };

		stbi_uc* data = stbi_load_gif_from_memory(
			fileBuffer.data(),
			static_cast<int>(fileBuffer.size()),
			&delays, &x, &y, &frames, &comp, 4
		);

		if (!data) return;

		std::vector<RawFrameData> tempFrames;
		tempFrames.reserve(frames);

		size_t frameSize{ static_cast<size_t>(x) * y * 4 };

		for (int i = 0; i < frames; ++i) {
			RawFrameData rawFrame;
			rawFrame.width = static_cast<unsigned>(x);
			rawFrame.height = static_cast<unsigned>(y);
			rawFrame.pixels.resize(frameSize);

			stbi_uc* currentFrameSource = data + (i * frameSize);
			std::copy(currentFrameSource, currentFrameSource + frameSize, rawFrame.pixels.begin());
			tempFrames.emplace_back(std::move(rawFrame));
		}

		stbi_image_free(data);
		if (delays) STBI_FREE(delays);

		{
			std::lock_guard<std::mutex> lock(s_framesMutex);
			s_rawFrames = std::move(tempFrames);
		}
	}

	void HostPanel::GenerateTextures(int limit)
	{
		std::lock_guard<std::mutex> lock(s_framesMutex);

		if (s_rawFrames.empty()) return;

		if (s_frames.size() >= s_rawFrames.size())
		{
			if (!s_rawFrames.empty()) {
				s_rawFrames.clear();
			}
			return;
		}

		int processedCount{ 0 };
		while (processedCount < limit && s_frames.size() < s_rawFrames.size())
		{
			size_t indexToLoad{ s_frames.size() };
			RawFrameData& raw{ s_rawFrames[indexToLoad] };

			tgui::Texture tguiTex;
			tguiTex.loadFromPixelData(
				{ raw.width, raw.height },
				raw.pixels.data()
			);
			s_frames.emplace_back(tguiTex);
			std::vector<uint8_t>().swap(raw.pixels);
			processedCount++;
		}
	}
}