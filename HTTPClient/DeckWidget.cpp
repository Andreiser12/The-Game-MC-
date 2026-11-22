module;
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <random>
#include <string>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

module DeckWidget;

DeckWidget::DeckWidget(DeckType deckType, uint8_t deckId) noexcept :
	m_deckType{ deckType }, m_deckId{ deckId }
{
	m_lastId = (m_deckType == DeckType::Ascending) ? m_lastId = 1u : m_lastId = 100u;
	CreateDeckWidget();

	RenderDeck();
}

void DeckWidget::CreateDeckWidget()
{
	m_deckPanel = tgui::Panel::create({ "100%", "100%" });

	m_deckPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
}

void DeckWidget::RenderDeck()
{
	m_deckPanel->removeAllWidgets();

	int cardsPlayed{ 0 };
	if (m_deckType == DeckType::Ascending)
		cardsPlayed = m_lastId - 1;
	else
		cardsPlayed = 100 - m_lastId;

	int visualLayers = cardsPlayed / 10;
	if (visualLayers > 5) visualLayers = 5;

	static tgui::Texture tguiTexture;
	static bool initialized{ false };

	if (!initialized)
	{
		sf::Image img;
		if (img.loadFromFile("assets/images/cardFrontSide.png"))
		{
			tguiTexture.loadFromPixelData(img.getSize(), img.getPixelsPtr());
		}
		initialized = true;
	}

	float offsetPercent{ 1.2f };
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> rotationDist(-2.0f, 2.0f);

	for (int i = 0; i < visualLayers; ++i)
	{
		auto bgCard = tgui::Picture::create(tguiTexture);

		bgCard->setSize({ "90%", "90%" });
		bgCard->setOrigin(0.5f, 0.5f);
		bgCard->setIgnoreMouseEvents(true);

		float shift{ (i + 1) * offsetPercent };
		bgCard->setPosition({
			tgui::String(50.0f + shift) + "%",
			tgui::String(50.0f + shift) + "%"
			});

		bgCard->setRotation(rotationDist(rng));

		bgCard->getRenderer()->setOpacity(0.9f - (0.05f * (visualLayers - 1 - i)));

		m_deckPanel->add(bgCard);
	}

	m_topCard = tgui::Picture::create(tguiTexture);
	m_topCard->setSize({ "90%", "90%" });

	float topShift{ visualLayers * offsetPercent };

	m_topCard->setPosition({
			tgui::String(50.0f + topShift) + "%",
			tgui::String(50.0f + topShift) + "%"
		});

	m_topCard->setOrigin(0.5f, 0.5f);
	m_topCard->getRenderer()->setOpacity(1.0f);
	m_deckPanel->add(m_topCard);

	m_topIdLabel = tgui::Label::create(tgui::String(m_lastId));
	m_topIdLabel->setPosition({
			tgui::String(50.0f + topShift) + "%",
			tgui::String(50.0f + topShift) + "%"
		});
	m_topIdLabel->setOrigin(0.5f, 0.5f);
	m_topIdLabel->setTextSize(20);
	m_topIdLabel->getRenderer()->setTextColor(tgui::Color::Black);
	m_topIdLabel->setIgnoreMouseEvents(true);
	m_deckPanel->add(m_topIdLabel);

	m_highlightOverlay = tgui::Panel::create({ "90%", "90%" });
	m_highlightOverlay->setPosition({
			tgui::String(50.0f + topShift) + "%",
			tgui::String(50.0f + topShift) + "%"
		});
	m_highlightOverlay->setOrigin(0.5f, 0.5f);
	m_highlightOverlay->getRenderer()->setRoundedBorderRadius(5);
	m_highlightOverlay->setVisible(false);
	m_highlightOverlay->setIgnoreMouseEvents(true);
	m_deckPanel->add(m_highlightOverlay);
}

DeckWidget::DeckType DeckWidget::GetDeckType() const noexcept
{
	return m_deckType;
}

uint8_t DeckWidget::GetLastId() const noexcept
{
	return m_lastId;
}

uint8_t DeckWidget::GetDeckId() const noexcept
{
	return m_deckId;
}

tgui::Panel::Ptr DeckWidget::GetDeckPanel() const noexcept
{
	return m_deckPanel;
}

void DeckWidget::SetLastId(uint8_t lastId) noexcept
{
	if (m_lastId == lastId) return;

	m_lastId = lastId;
	RenderDeck();
}

void DeckWidget::SetHighlight(bool canPlace, bool isBackwards) noexcept
{
	if (!m_highlightOverlay) return;

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
		m_highlightOverlay->getRenderer()->setBackgroundColor(tgui::Color(100, 100, 100, 50));
	}
}

void DeckWidget::ClearHighlight() noexcept
{
	if (m_highlightOverlay) m_highlightOverlay->setVisible(false);
}
