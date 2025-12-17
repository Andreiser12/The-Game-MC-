#include "CardWidget.h"

CardWidget::CardWidget(unsigned id, bool isOwner) :
	m_id{id},
	m_isOwner{isOwner},
	m_faceState{isOwner ? FaceState::FaceUp : FaceState::FaceUp}
{
	CreateCardWidget();
}

tgui::Panel::Ptr CardWidget::GetWidget() const
{
	return m_cardPanel;
}

unsigned CardWidget::GetId() const
{
	return m_id;
}

void CardWidget::SetFaceState(FaceState state)
{
	m_faceState = state;
	UpdateCardAppearance();
}

void CardWidget::SetPosition(const tgui::String& x, const tgui::String& y)
{
	m_cardPanel->setPosition(x, y);
}

void CardWidget::SetSize(const tgui::String& width, const tgui::String& height)
{
	m_cardPanel->setSize(width, height);
}

void CardWidget::SetDraggable(bool draggable)
{
	if (draggable && m_isOwner)
	{
		m_cardPanel->setMouseCursor(tgui::Cursor::Type::Hand);
	}
	else
	{
		m_cardPanel->setMouseCursor(tgui::Cursor::Type::Arrow);
	}
}

void CardWidget::CreateCardWidget()
{
	m_cardPanel = tgui::Panel::create({ "100%", "100%" });
	m_cardPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
	m_cardPanel->getRenderer()->setBorders(2);
	m_cardPanel->getRenderer()->setBorderColor(tgui::Color::Black);
	m_cardPanel->getRenderer()->setRoundedBorderRadius(5);

	m_idLabel = tgui::Label::create(tgui::String(m_id));
	m_idLabel->setPosition({ "50%", "50%" });
	m_idLabel->setOrigin(0.5f, 0.5f);
	m_idLabel->setTextSize(20);
	m_idLabel->getRenderer()->setTextColor(tgui::Color::Black);
	m_idLabel->setIgnoreMouseEvents(true);

	m_cardPanel->add(m_idLabel);
	UpdateCardAppearance();
}

void CardWidget::UpdateCardAppearance()
{
	if (m_faceState == FaceState::FaceUp)
	{
		m_idLabel->setVisible(true);
		m_cardPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
	}
	else
	{
		m_idLabel->setVisible(false);
		m_cardPanel->getRenderer()->setBackgroundColor(tgui::Color(180, 30, 30));
	}
}
