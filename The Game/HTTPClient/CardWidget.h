#pragma once
#include "DeckWidget.h"
#include <optional>
#include <functional>

class CardWidget
{
public:
	enum class FaceState
	{
		FaceUp,
		FaceDown
	};
	
	CardWidget(uint8_t id, bool isOwner);

	tgui::Panel::Ptr GetWidget() const;
	uint8_t GetId() const;

	void SetFaceState(FaceState state);
	void SetPosition(const tgui::String& x, const tgui::String& y);
	void SetSize(const tgui::String& width, const tgui::String& height);
	void SetDraggable(bool draggable);
	void UpdateDragPosition(tgui::Vector2f mousePos);
	void CancelDrag();
	void CheckOnDeck(std::shared_ptr<DeckWidget> deckWidget);
	bool IsPlacedOnDeck() const;
	void MarkAsPlaced();

	std::function<bool(uint8_t cardId, std::shared_ptr<DeckWidget>)> onCardPlaced;

private:
	void CreateCardWidget();
	void UpdateCardAppearance();
	bool IsNearDeck(const tgui::Panel::Ptr& deck);

private:
	uint8_t m_id;
	bool m_isOwner;
	FaceState m_faceState;

	tgui::Panel::Ptr m_cardPanel;
	tgui::Label::Ptr m_idLabel;
	tgui::Picture::Ptr m_frontSide;
	tgui::Picture::Ptr m_backSide;

	bool m_isDragging{ false };
	bool m_isOnDeck{ false };
	bool m_isPlacedOnDeck{ false };
	tgui::Vector2f m_dragOffset;
	tgui::Vector2f m_originalPosition;
	std::shared_ptr<DeckWidget> m_targetDeck{ nullptr };
};

