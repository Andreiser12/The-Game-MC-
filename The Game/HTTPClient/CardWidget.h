#pragma once
#include "TGUI/TGUI.hpp"
#include <string>

class CardWidget
{
public:
	enum class FaceState
	{
		FaceUp,
		FaceDown
	};
	
	CardWidget(unsigned id, bool isOwner);

	tgui::Panel::Ptr GetWidget() const;
	unsigned GetId() const;

	void SetFaceState(FaceState state);
	void SetPosition(const tgui::String& x, const tgui::String& y);
	void SetSize(const tgui::String& width, const tgui::String& height);
	void SetDraggable(bool draggable);

private:
	void CreateCardWidget();
	void UpdateCardAppearance();

private:
	unsigned m_id;
	bool m_isOwner;
	FaceState m_faceState;

	tgui::Panel::Ptr m_cardPanel;
	tgui::Label::Ptr m_idLabel;
};

