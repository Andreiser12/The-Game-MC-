module;
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <cmath>
#include <algorithm>
#include <memory>
#include <functional>
#include <string>
#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

module CardWidget;

CardWidget::CardWidget(uint8_t id, bool isOwner) noexcept :
	m_id{ id },
	m_isOwner{ isOwner },
	m_faceState{ isOwner ? FaceState::FaceUp : FaceState::FaceDown }
{
	CreateCardWidget();
}

tgui::Panel::Ptr CardWidget::GetWidget() const noexcept
{
	return m_cardPanel;
}

uint8_t CardWidget::GetId() const noexcept
{
	return m_id;
}

void CardWidget::SetFaceState(FaceState state) noexcept
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
	m_cardPanel->setSize({ width, height });
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

void CardWidget::UpdateDragPosition(tgui::Vector2f mousePos)
{
	if (!m_isDragging || !m_isOwner) return;
	tgui::Vector2f parentGlobal = m_cardPanel->getParent()->getAbsolutePosition();
	tgui::Vector2f relativePos = mousePos - parentGlobal - m_dragOffset;
	m_cardPanel->setPosition(relativePos);
}

void CardWidget::CancelDrag()
{
	if (!m_isDragging) return;

	m_isDragging = false;
	m_isOnDeck = false;
	m_targetDeck = nullptr;
	m_cardPanel->setPosition(m_originalPosition);
}

void CardWidget::CheckOnDeck(const std::shared_ptr<DeckWidget>& deckWidget)
{
	if (!m_isDragging) return;

	if (IsNearDeck(deckWidget->GetDeckPanel()))
	{
		m_isOnDeck = true;
		m_targetDeck = deckWidget;
	}
	else
	{
		if (m_targetDeck == deckWidget)
		{
			m_isOnDeck = false;
			m_targetDeck = nullptr;
		}
	}
}

bool CardWidget::IsPlacedOnDeck() const noexcept
{
	return m_isPlacedOnDeck;
}

void CardWidget::MarkAsPlaced()
{
	m_isPlacedOnDeck = true;
}

bool CardWidget::IsDragging() const noexcept
{
	return m_isDragging;
}

void CardWidget::CreateCardWidget()
{
	m_cardPanel = tgui::Panel::create({ "100%", "100%" });
	m_cardPanel->getRenderer()->setBorders(2);
	m_cardPanel->getRenderer()->setBorderColor(tgui::Color::Black);
	m_cardPanel->getRenderer()->setRoundedBorderRadius(5);

	m_frontSide = tgui::Picture::create("assets/images/cardFrontSide.png");
	m_frontSide->setSize({ "100%", "100%" });
	m_frontSide->setPosition({ "0%", "0%" });
	m_cardPanel->add(m_frontSide);
	m_frontSide->moveToBack();

	m_backSide = tgui::Picture::create("assets/images/cardBackside.png");
	m_backSide->setSize({ "100%", "100%" });
	m_backSide->setPosition({ "0%", "0%" });
	m_cardPanel->add(m_backSide);

	m_idLabel = tgui::Label::create(tgui::String(m_id));
	m_idLabel->setPosition({ "50%", "50%" });
	m_idLabel->setOrigin(0.5f, 0.5f);
	m_idLabel->setTextSize(20);
	m_idLabel->getRenderer()->setTextColor(tgui::Color::Black);
	m_idLabel->setIgnoreMouseEvents(true);
	m_cardPanel->add(m_idLabel);

	if (m_isOwner)
	{
		m_cardPanel->onMousePress([this](tgui::Vector2f mousePos) {

			if (onPickup) onPickup();
			m_isDragging = true;
			m_dragOffset = mousePos;
			m_originalPosition = m_cardPanel->getPosition();
			m_cardPanel->moveToFront();
			});
		m_cardPanel->onMouseRelease([this]() {

			if (!m_isDragging) return;

			m_isDragging = false;
			if (m_isOnDeck && m_targetDeck && onCardPlaced)
			{
				if (onCardPlaced(m_id, m_targetDeck))
				{
					m_isPlacedOnDeck = true;
					m_cardPanel->setVisible(false);
				}
				else
				{
					m_cardPanel->setPosition(m_originalPosition);
				}
				m_isOnDeck = false;
				m_targetDeck = nullptr;
			}
			else
			{
				m_cardPanel->setPosition(m_originalPosition);
			}
			});

		m_cardPanel->setMouseCursor(tgui::Cursor::Type::Hand);
	}
	else
	{
		m_cardPanel->setMouseCursor(tgui::Cursor::Type::Arrow);
	}

	UpdateCardAppearance();
}

void CardWidget::UpdateCardAppearance() noexcept
{
	if (m_faceState == FaceState::FaceUp)
	{
		m_frontSide->setVisible(true);
		m_idLabel->setVisible(true);
		m_backSide->setVisible(false);
	}
	else
	{
		m_frontSide->setVisible(false);
		m_idLabel->setVisible(false);
		m_backSide->setVisible(true);
	}
}

bool CardWidget::IsNearDeck(const tgui::Panel::Ptr& deck) const noexcept
{
	tgui::Vector2f cardPosition{ m_cardPanel->getPosition() };
	tgui::Vector2f cardSize{ m_cardPanel->getSize() };
	tgui::Vector2f cardCenter{ cardPosition + (cardSize / 2.0f) };

	tgui::Vector2f deckPosition{ deck->getPosition() };
	tgui::Vector2f deckSize{ deck->getSize() };
	tgui::Vector2f deckCenter{ deckPosition + (deckSize / 2.0f) };

	const float dx{ cardCenter.x - deckCenter.x };
	const float dy{ cardCenter.y - deckCenter.y };
	const float distance{ std::sqrt(dx * dx + dy * dy) };
	const float threshold{ std::max(cardSize.x, cardSize.y) * 0.8f };

	return distance < threshold;
}
