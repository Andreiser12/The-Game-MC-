module;
#include <string>
#include <random>
#include <cmath>
#include <memory>
#include <cstdint>

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

export module DeckWidget;

export class DeckWidget
{
public:
	enum class DeckType : uint8_t
	{
		Ascending,
		Descending
	};

	explicit DeckWidget(DeckType deckType, uint8_t deckId) noexcept;

	DeckWidget(const DeckWidget&) = delete;
	DeckWidget& operator = (const DeckWidget&) = delete;
	DeckWidget(DeckWidget&&) = delete;
	DeckWidget& operator = (DeckWidget&&) = delete;

	void CreateDeckWidget();
	void RenderDeck();

	[[nodiscard]] DeckType GetDeckType() const noexcept;
	[[nodiscard]] uint8_t GetLastId() const noexcept;
	[[nodiscard]] uint8_t GetDeckId() const noexcept;
	[[nodiscard]] tgui::Panel::Ptr GetDeckPanel() const noexcept;

	void SetLastId(uint8_t lastId) noexcept;
	void SetHighlight(bool canPlace, bool isBackwards = false) noexcept;
	void ClearHighlight() noexcept;

private:
	uint8_t m_lastId{ 0 };
	uint8_t m_deckId;
	DeckType m_deckType;

	tgui::Panel::Ptr m_deckPanel;
	tgui::Panel::Ptr m_highlightOverlay;
	tgui::Picture::Ptr m_topCard;
	tgui::Label::Ptr m_topIdLabel;
};