#include "PileWidget.h"

PileWidget::PileWidget()
{
	m_pilePanel = tgui::Panel::create({"100%", "100%"});
	m_pilePanel->getRenderer()->setBorderColor(tgui::Color::Black);
	m_pilePanel->getRenderer()->setBorders(2);
	m_pilePanel->getRenderer()->setRoundedBorderRadius(5);

	m_backsidePicture = tgui::Picture::create("assets/images/cardBackside.png");
	m_backsidePicture->setSize({ "100%", "100%" });
	m_backsidePicture->setPosition({ "0%", "0%" });
	m_backsidePicture->setIgnoreMouseEvents(true);
	m_pilePanel->add(m_backsidePicture);
	m_backsidePicture->moveToBack();
}

tgui::Panel::Ptr PileWidget::GetPilePanel() const
{
	return m_pilePanel;
}
