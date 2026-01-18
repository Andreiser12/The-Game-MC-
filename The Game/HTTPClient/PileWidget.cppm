module;
#include <memory>
#include <TGUI/TGUI.hpp>

export module PileWidget;

export class PileWidget
{
public:
	explicit PileWidget() noexcept;

	PileWidget(const PileWidget&) = delete;
	PileWidget& operator= (const PileWidget&) = delete;
	PileWidget(PileWidget&&) = delete;
	PileWidget& operator = (PileWidget&&) = delete;

	[[nodiscard]] tgui::Panel::Ptr GetPilePanel() const noexcept;
	void UpdatePile(int cardCount) noexcept;

public:
	static inline constexpr uint8_t kPileSize{ 50u };

private:
	tgui::Panel::Ptr m_pilePanel;
	int m_lastCardCount{ -1 };
};