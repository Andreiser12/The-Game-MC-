module;
#include <memory>
#include <random>
#include <algorithm>
#include <TGUI/TGUI.hpp>

export module PileWidget;

export class PileWidget
{
public:
	PileWidget();

	tgui::Panel::Ptr GetPilePanel() const;
	void UpdatePile(int cardCount);

private:
	tgui::Panel::Ptr m_pilePanel;
	int m_lastCardCount{ -1 };
	tgui::Picture::Ptr m_backsidePicture;
};

