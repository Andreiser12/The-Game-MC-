#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <SFML/System.hpp>

#include "ApiClient.h"
#include "Utils.h"
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

private:
	unsigned m_lastMessageId{ 0u };
	tgui::Group::Ptr m_root;
	tgui::ChatBox::Ptr m_chat;
	sf::Clock m_chatClock;
	ApiClient& m_apiClient;

	inline static constexpr std::array<PlayerSlot, 2> k_slots2{ {
		{45.0f, 75.0f},
		{45.0f, 13.0f}} };

	inline static constexpr std::array<PlayerSlot, 3> k_slots3{ {
		{45.0f, 75.0f},
		{72.5f, 42.0f},
		{13.0f, 42.0f}} };

	inline static constexpr std::array<PlayerSlot, 4> k_slots4{ {
		{45.0f, 75.0f},
		{72.5f, 46.0f},
		{45.0f, 13.0f},
		{13.0f, 45.0f}} };

	inline static constexpr std::array<PlayerSlot, 5> k_slots5{ {
		{55.f, 25.f},
		{15.f, 35.f},
		{15.f, 70.f},
		{85.f, 75.f},
		{85.f, 35.f}} };

	inline static constexpr uint8_t kmaxChars{ 15u };
};