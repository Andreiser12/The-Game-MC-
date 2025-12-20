#pragma once
#include "TGUI/TGUI.hpp"

class PileWidget
{
public:
	PileWidget();

	tgui::Panel::Ptr GetPilePanel() const;

private:
	tgui::Panel::Ptr m_pilePanel;
	tgui::Picture::Ptr m_backsidePicture;
};

