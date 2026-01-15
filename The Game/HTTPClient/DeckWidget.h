#pragma once
#include "TGUI/TGUI.hpp"
#include <string>
#include <random>
#include <cmath>
#include <SFML/Graphics/Image.hpp>

class DeckWidget
{
public:
	enum class DeckType
	{
		Ascending,
		Descending
	};

	DeckWidget(DeckType deckType, uint8_t deckId);
	void CreateDeckWidget();
	void RenderDeck();

	DeckType GetDeckType() const;
	uint16_t GetLastId() const;
	uint8_t GetDeckId() const;
	tgui::Panel::Ptr GetDeckPanel() const;

	void SetLastId(uint16_t lastId);
	void SetHighlight(bool canPlace, bool isBackwards = false);
	void ClearHighlight();

private:
	uint16_t m_lastId{ 0 };
	uint8_t m_deckId;
	DeckType m_deckType;
	
	tgui::Panel::Ptr m_deckPanel;
	tgui::Panel::Ptr m_highlightOverlay;
	tgui::Picture::Ptr m_topCard;
	tgui::Label::Ptr m_topIdLabel;

};

