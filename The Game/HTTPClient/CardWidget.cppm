module;
#include <optional>
#include <functional>
#include <memory>
#include <string>
#include <cstdint>

#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

export module CardWidget;

import DeckWidget;

export class CardWidget
{
public:
	enum class FaceState
	{
		FaceUp,
		FaceDown
	};
	
	explicit CardWidget(uint8_t id, bool isOwner) noexcept;

	CardWidget(const CardWidget&) = delete;
	CardWidget& operator=(const CardWidget&) = delete;
	CardWidget(CardWidget&&) = delete;
	CardWidget& operator=(CardWidget&&) = delete;

	[[nodiscard]] tgui::Panel::Ptr GetWidget() const noexcept;
	[[nodiscard]] uint8_t GetId() const noexcept;
	[[nodiscard]] bool IsPlacedOnDeck() const noexcept;
	[[nodiscard]] bool IsDragging() const noexcept;

	void SetFaceState(FaceState state) noexcept;
	void SetPosition(const tgui::String& x, const tgui::String& y);
	void SetSize(const tgui::String& width, const tgui::String& height);
	void SetDraggable(bool draggable);
	void UpdateDragPosition(tgui::Vector2f mousePos);
	void CancelDrag();
	void CheckOnDeck(const std::shared_ptr<DeckWidget>& deckWidget);
	void MarkAsPlaced();
	
	std::function<void()> onPickup;
	std::function<bool(uint8_t cardId, std::shared_ptr<DeckWidget>)> onCardPlaced;

private:
	void CreateCardWidget();
	void UpdateCardAppearance() noexcept;
	bool IsNearDeck(const tgui::Panel::Ptr& deck) const noexcept;

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

