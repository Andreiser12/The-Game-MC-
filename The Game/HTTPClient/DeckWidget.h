#pragma once
#include "TGUI/TGUI.hpp"
#include <string>

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

	DeckType GetDeckType() const;
	uint16_t GetLastId() const;
	uint8_t GetDeckId() const;
	tgui::Panel::Ptr GetDeckPanel() const;

	void SetLastId(uint16_t lastId);

private:
	uint16_t m_lastId;
	uint8_t m_deckId;
	DeckType m_deckType;
	
	tgui::Panel::Ptr m_deckPanel;
	tgui::Label::Ptr m_topIdLabel;
	tgui::Picture::Ptr m_frontSide;
};

