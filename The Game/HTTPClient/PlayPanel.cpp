#include "PlayPanel.h"

tgui::Panel::Ptr PlayPanel::Create(tgui::Gui& gui,
	std::function<void()> onJoin,
	std::function<void()> onHost)
{
	auto panel = tgui::Panel::create({"60%", "40%"});
	panel->setPosition({ "50%", "50%" });
	panel->setOrigin(0.5f, 0.5f);

	auto titleLabel = tgui::Label::create("Select");
	titleLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	titleLabel->setPosition({ "50%", "20%" });
	titleLabel->setOrigin(0.5f, 0.5f);
	titleLabel->setTextSize(26);
	panel->add(titleLabel);

	return panel;
}

