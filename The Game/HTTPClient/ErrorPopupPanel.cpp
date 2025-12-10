#include "ErrorPopupPanel.h"

tgui::Panel::Ptr ErrorPopupPanel::Create(tgui::Gui& gui, std::function<void()> onOk)
{
    auto panel = tgui::Panel::create();

    return panel;
}
