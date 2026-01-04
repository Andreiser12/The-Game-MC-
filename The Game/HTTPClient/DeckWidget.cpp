#include "DeckWidget.h"

DeckWidget::DeckWidget(DeckType deckType, uint8_t deckId) : m_deckType{deckType}, m_deckId{deckId}
{
	m_lastId = (m_deckType == DeckType::Ascending) ? m_lastId = 1u : m_lastId = 100u;
	CreateDeckWidget();
}

void DeckWidget::CreateDeckWidget()
{
	m_deckPanel = tgui::Panel::create({ "100%", "100%" });
	m_deckPanel->getRenderer()->setBorderColor(tgui::Color::Black);
	m_deckPanel->getRenderer()->setBorders(2);
	m_deckPanel->getRenderer()->setRoundedBorderRadius(5);

	m_highlightOverlay = tgui::Panel::create({ "100%", "100%" });
	m_highlightOverlay->setPosition({ "0%", "0%" });
	m_highlightOverlay->getRenderer()->setRoundedBorderRadius(5);
	m_highlightOverlay->setVisible(false);
	m_highlightOverlay->setIgnoreMouseEvents(true);
	m_deckPanel->add(m_highlightOverlay);

	m_frontSide = tgui::Picture::create("assets/images/cardFrontSide.png");
	m_frontSide->setSize({ "100%", "100%" });
	m_frontSide->setPosition({ "0%", "0%" });
	m_deckPanel->add(m_frontSide);
	m_frontSide->moveToBack();

	m_topIdLabel = tgui::Label::create(tgui::String(m_lastId));
	m_topIdLabel->setPosition({ "50%", "50%" });
	m_topIdLabel->setOrigin(0.5f, 0.5f);
	m_topIdLabel->setTextSize(20);
	m_topIdLabel->getRenderer()->setTextColor(tgui::Color::Black);
	m_topIdLabel->setIgnoreMouseEvents(true);
	m_deckPanel->add(m_topIdLabel);
}

DeckWidget::DeckType DeckWidget::GetDeckType() const
{
	return m_deckType;
}

uint16_t DeckWidget::GetLastId() const
{
	return m_lastId;
}

uint8_t DeckWidget::GetDeckId() const
{
	return m_deckId;
}

tgui::Panel::Ptr DeckWidget::GetDeckPanel() const
{
	return m_deckPanel;
}

void DeckWidget::SetLastId(uint16_t lastId)
{
	m_lastId = lastId;
	m_topIdLabel->setText(tgui::String(lastId));
}

void DeckWidget::SetHighlight(bool canPlace, bool isBackwards)
{
	m_highlightOverlay->setVisible(true);

	if (canPlace)
	{
		if (isBackwards)
		{
			m_highlightOverlay->getRenderer()->setBackgroundColor(tgui::Color(148, 0, 211, 80));
		}
		else
		{
			m_highlightOverlay->getRenderer()->setBackgroundColor(tgui::Color(0, 255, 0, 80));
		}
	}
	else
	{
		m_highlightOverlay->getRenderer()->setBackgroundColor(tgui::Color(100, 100, 100, 100));
	}
}

void DeckWidget::ClearHighlight()
{
	m_highlightOverlay->setVisible(false);
	m_deckPanel->getRenderer()->setBackgroundColor(tgui::Color::Black);
}
