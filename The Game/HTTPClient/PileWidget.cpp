#include "PileWidget.h"


PileWidget::PileWidget()
{
	m_pilePanel = tgui::Panel::create({ "100%", "100%" });
	m_pilePanel->getRenderer()->setBorderColor(tgui::Color::Black);
	m_pilePanel->getRenderer()->setBorders(2);
	m_pilePanel->getRenderer()->setRoundedBorderRadius(5);

	m_backsidePicture = tgui::Picture::create("assets/images/cardBackside.png");
	m_backsidePicture->setSize("100%", "100%");
	m_backsidePicture->setPosition("0%", "0%");
	m_backsidePicture->setIgnoreMouseEvents(true);
	m_pilePanel->add(m_backsidePicture);
	m_backsidePicture->moveToBack();
}

tgui::Panel::Ptr PileWidget::GetPilePanel() const
{
	return m_pilePanel;
}

void PileWidget::UpdatePile(int cardCount)
{
	if (m_lastCardCount == cardCount) return;
	m_lastCardCount = cardCount;

	m_pilePanel->removeAllWidgets();

	if (cardCount <= 0) return;

	int visualLayers = (cardCount > 1) ? 3 : 1;

	constexpr float offset{ 5.0f };

	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(-2.0f, 2.0f);

	for (int i = visualLayers - 1; i >= 0; --i)
	{
		auto cardPic = tgui::Picture::create("assets/images/cardBackside.png");
		cardPic->setSize("100%", "100%");
		cardPic->setIgnoreMouseEvents(true);
		cardPic->setOrigin(0.5f, 0.5f);

		float shift = i * offset;
		cardPic->setPosition({ tgui::String(50 + shift) + "%", tgui::String(50 + shift) + "%" });

		if (i > 0)
		{
			cardPic->setRotation(dist(rng));
			cardPic->getRenderer()->setOpacity(0.85f);
		}

		m_pilePanel->add(cardPic);
	}
}