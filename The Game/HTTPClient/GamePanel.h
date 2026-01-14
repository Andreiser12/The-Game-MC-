#pragma once
#include <SFML/System.hpp>

#include "ApiClient.h"
#include "Utils.h"
#include "CardWidget.h"
#include "DeckWidget.h"
#include "PileWidget.h"
#include <span>

struct PlayerSlot
{
	float xPercent;
	float yPercent;
};

struct PlayerSlotWidget
{
	std::string username;
	tgui::Panel::Ptr panel;
	tgui::Label::Ptr nameLabel;
	std::vector<std::shared_ptr<CardWidget>> cards;
	bool isActive{ true };
};

using SlotArray = std::span<const PlayerSlot>;

class GamePanel
{
public:
	explicit GamePanel(tgui::Gui& gui, ApiClient& api, std::function<void()> onExit,
		std::function<void()> onLocalPlayerLeft);
	void UpdateGame();
	void UpdateDecks();
	void UpdateEndTurnButton();
	void UpdateChat();
	void UpdateTableState();
	void UpdateTurnState();
	void UpdatePlayerStates();
	void SetupPlayerSlots();
	void OnGameStarted();
	void HandleMouseEvent(const sf::Event& event);
	void ResetAllDrags();
	void RemovePlacedCards();
	~GamePanel();

	void OnTurnTimeout();

	void MarkPlayerAsLeft(const std::string& username);


private:
	inline SlotArray GetSlotsForPlayerCount(size_t count)
	{
		switch (count)
		{
			case 2: return k_slots2;
			case 3: return k_slots3;
			case 4: return k_slots4;
			case 5: return k_slots5;
			default: return {};
		}
	}
	void CreatePlayerHand(size_t playerIndex, const PlayerSlot& slot, std::vector<std::shared_ptr<CardWidget>>& outCards);
	void CreateMyHand();
	void ClearMyHand();
	void RebuildMyHand();
	void HandleGameResult(GameResult result);
	void ShowGameResultOverlay(GameResult result);
	bool CanMakeAnyMove() const;

private:
	unsigned m_lastMessageId{ 0u };
	tgui::Gui& m_gui;
	tgui::Group::Ptr m_root;
	std::unordered_map<std::string, PlayerSlotWidget> m_playerSlots;
	tgui::ChatBox::Ptr m_chat;
	sf::Clock m_chatClock;
	ApiClient& m_apiClient;
	std::function<void()> m_onExitToMenu;
	std::function<void()> m_onLocalPlayerLeft;

	inline static constexpr std::array<PlayerSlot, 2> k_slots2{ {
		{45.0f, 75.0f},
		{45.0f, 15.0f}} };

	inline static constexpr std::array<PlayerSlot, 3> k_slots3{ {
		{45.0f, 75.0f},
		{72.5f, 42.0f},
		{13.0f, 42.0f}} };

	inline static constexpr std::array<PlayerSlot, 4> k_slots4{ {
		{45.0f, 75.0f},
		{72.5f, 46.0f},
		{45.0f, 15.0f},
		{13.0f, 45.0f}} };

	inline static constexpr std::array<PlayerSlot, 5> k_slots5{ {
		{43.0f, 73.0f},
		{72.0f, 46.0f},
		{58.0f, 16.0f},
		{31.0f, 15.0f},
		{14.0f, 46.0f}} };
	
	inline static constexpr uint8_t kmaxChars{ 15u };
	std::vector<std::shared_ptr<CardWidget>> m_myCards;
	std::array<std::shared_ptr<DeckWidget>, 4> m_decks;
	std::shared_ptr<PileWidget> m_pileWidget;
	std::optional<PlayerSlot> m_mySlot;
	tgui::Button::Ptr m_endTurnButton;
	tgui::Button::Ptr m_leaveMatchButton;

	bool m_isMyTurn{ false };
	bool m_gameFinished{ false };

	tgui::Label::Ptr m_timerLabel;
	float m_timeLeft{60.0f};
	sf::Clock m_deltaTimeClock;

	sf::Music m_gameMusic;

	sf::SoundBuffer m_ExitButtonBuffer;
	sf::SoundBuffer m_clickBuffer;
	sf::SoundBuffer m_leaveBuffer;
	sf::SoundBuffer m_cardFlipBuffer;
	sf::SoundBuffer m_validMoveBuffer;
	sf::SoundBuffer m_specialMoveBuffer;
	sf::SoundBuffer m_errorMoveBuffer;
	sf::SoundBuffer m_timerTickBuffer;

	sf::Sound m_uiSound;
	sf::Sound m_cardSound;
	sf::Sound m_feedbackSound;
	sf::Sound m_timerSound;
	sf::Sound m_ExitButtonSound;

	int m_lastTimerSecond{ -1 };
	int m_lastHighlightDeckId{ -1 };
	bool m_wasHighlighting{ false };

	float m_normalMusicVolume{ 50.0f };
	bool m_isMusicDimmed{ false };
};