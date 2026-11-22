module;
#include <memory>
#include <random>
#include <TGUI/TGUI.hpp>

module PileWidget;

PileWidget::PileWidget() noexcept
{
	m_pilePanel = tgui::Panel::create({ "100%", "100%" });
	m_pilePanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
	UpdatePile(kPileSize);
}

tgui::Panel::Ptr PileWidget::GetPilePanel() const noexcept
{
	return m_pilePanel;
}

void PileWidget::UpdatePile(int cardCount) noexcept
{
	if (m_lastCardCount == cardCount) return;
	m_lastCardCount = cardCount;

	m_pilePanel->removeAllWidgets();

	if (cardCount <= 0)
	{
		m_pilePanel->setVisible(false);
		return;
	}

	m_pilePanel->setVisible(true);

	int visualLayers{};
	if (cardCount == 1) visualLayers = 1;
	else if (cardCount <= 10) visualLayers = 2;
	else if (cardCount <= 30) visualLayers = 3;
	else visualLayers = 5;

	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> rotationDist(-2.0f, 2.0f);
	constexpr float offsetPercent{ 1.2f };

	for (int i = 0; i < visualLayers; ++i)
	{
		auto cardBackside = tgui::Picture::create("assets/images/cardBackside.png");

		cardBackside->setSize({ "90%", "90%" });
		cardBackside->setIgnoreMouseEvents(true);
		cardBackside->setOrigin(0.5f, 0.5f);

		const float shift{ i * offsetPercent };
		cardBackside->setPosition({
			tgui::String(50.0f + shift) + "%",
			tgui::String(50.0f + shift) + "%"
			});

		if (i < visualLayers - 1)
		{
			cardBackside->setRotation(rotationDist(rng));
			cardBackside->getRenderer()->setOpacity(0.9f);
		}
		else
		{
			cardBackside->setRotation(0.f);
			cardBackside->getRenderer()->setOpacity(1.f);
		}

		m_pilePanel->add(cardBackside);
	}
}