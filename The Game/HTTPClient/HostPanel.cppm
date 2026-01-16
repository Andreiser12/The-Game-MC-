module;
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include "TGUI/Core.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Backend/SFML-Graphics.hpp"
#include <SFML/Audio.hpp>

export module HostPanel;

import <functional>;
import <string>;
import ApiClient;
import <mutex>;

export struct RawFrameData {
    unsigned int width;
    unsigned int height;
    std::vector<uint8_t> pixels;
    int delayMs;
};

export class HostPanel
{
public:
    static tgui::Panel::Ptr Create(tgui::Gui& gui,
        ApiClient& apiClient,
        sf::Sound& btnSound,
        const std::string& lobbyId,
        std::function<bool(ApiClient&)> onLeave,
        std::function<bool(ApiClient&)> onStart);

    static void RefreshPlayersList(ApiClient& apiClient);
    static void UpdateStartButton();
    static inline tgui::ListBox::Ptr GetPlayersListBox() { return playersListBox; }

    static void UpdateBackground(float deltaTime);
    //static void PreloadImages();
    static void LoadGifDataAsync();
    static void GenerateTextures(int limit = 1000);

private:
    static inline tgui::ListBox::Ptr playersListBox{ nullptr };
    static inline tgui::Button::Ptr startButton{ nullptr };
    static inline int s_hostId{ -1 };
    static inline int s_playerId{ -1 };
    static inline std::vector<RawFrameData> s_rawFrames;
    static inline std::mutex s_framesMutex;
};