#include "PlayPanel.h"

tgui::Panel::Ptr PlayPanel::Create(tgui::Gui& gui,
	std::function<void()> onJoin,
	std::function<void()> onHost)
{
	auto panel = tgui::Panel::create();

	return panel;
}

