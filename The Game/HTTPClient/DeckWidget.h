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

	DeckWidget(DeckType deckType);
	void CreateDeckWidget();

	DeckType GetDeckType() const;
	uint16_t GetLastId() const;
	tgui::Panel::Ptr GetDeckPanel() const;

	void SetLastId(uint16_t lastId);

private:
	uint16_t m_lastId;
	DeckType m_deckType;
	
	tgui::Panel::Ptr m_deckPanel;
	tgui::Label::Ptr m_topIdLabel;
	tgui::Picture::Ptr m_frontSide;
};

