#pragma once
#include <SFML/System.hpp>

#include "ApiClient.h"
#include "Utils.h"
#include "CardWidget.h"
#include "DeckWidget.h"
#include "PileWidget.h"

#include <vector>
#include <memory>
#include <array>
#include <span>

struct PlayerSlot
{
	float xPercent;
	float yPercent;
};

using SlotArray = std::span<const PlayerSlot>;

class GamePanel
{
public:
	GamePanel(tgui::Gui& gui, ApiClient& api);
	void Update();
	void SetupPlayerSlots();
	void OnGameStarted();
	void HandleMouseEvent(const sf::Event& event);
	void ResetAllDrags();
	void RemovePlacedCards();

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
	void CreatePlayerHand(size_t playerIndex, const PlayerSlot& slot);
	void CreateMyHand(const PlayerSlot& slot);

private:
	unsigned m_lastMessageId{ 0u };
	tgui::Group::Ptr m_root;
	tgui::ChatBox::Ptr m_chat;
	sf::Clock m_chatClock;
	ApiClient& m_apiClient;

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
};