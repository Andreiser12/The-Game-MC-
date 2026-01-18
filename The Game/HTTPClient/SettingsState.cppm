export module SettingsState;

#include <SFML/System/Vector2.hpp>
import <array>;
import <cstdint>;


export struct SettingsState
{
	static constexpr std::array<sf::Vector2u, 11> Resolutions{
		sf::Vector2u{640, 480},
		{800, 600},
		{1024, 768},
		{1280, 720},
		{1280, 800},
		{1366, 768},
		{1440, 900},
		{1680, 1050},
		{1920, 1080},
		{1920, 1200},
		{2560, 1440}
	};

	static constexpr std::array<uint8_t, 5> Framerates{
		30u, 60u, 120u, 144u, 180u
	};

	uint8_t resolutionIndex{ 8u };
	uint8_t framerateIndex{ 1u };
	bool fullscreen{ true };

	float masterVolume{ 100.0f };
	float buttonsVolume{ 100.0f };
	float backgroundVolume{ 100.0f };
};