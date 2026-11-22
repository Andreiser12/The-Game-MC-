module;
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <SFML/Audio.hpp>
#include <string>
#include <functional>
#include <mutex>

export module HostPanel;

import ApiClient;

export struct RawFrameData {
    unsigned width{};
    unsigned height{};
    std::vector<uint8_t> pixels;
    int delayMs{};
};

export namespace HostPanel
{
    tgui::Panel::Ptr Create(tgui::Gui& gui,
        ApiClient& apiClient,
        sf::Sound& btnSound,
        const std::string& lobbyId,
        std::function<bool(ApiClient&)>&& onLeave,
        std::function<bool(ApiClient&)>&& onStart);

    void RefreshPlayersList(ApiClient& apiClient);
    void UpdateStartButton();
    tgui::ListBox::Ptr GetPlayersListBox();

    void UpdateBackground(float deltaTime);
    void LoadGifDataAsync();
    void GenerateTextures(int limit);
};