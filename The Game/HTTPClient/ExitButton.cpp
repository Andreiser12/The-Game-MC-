#include "ExitButton.h"

ExitButton::ExitButton(sf::RenderWindow& win, tgui::Gui& g)
    : window(win), gui(g), dialogVisible(false) {
}

void ExitButton::Create(float x, float y, float width, float height)
{
    SetupExitButton(x, y, width, height);
}

void ExitButton::SetupExitButton(float x, float y, float width, float height)
{
    exitButton = tgui::Button::create("Exit");
    exitButton->setSize(width, height);
    exitButton->setPosition(x, y);
    exitButton->onPress([this] { ShowConfirmDialog(); });
    gui.add(exitButton, "ExitButton");
}

void ExitButton::ShowConfirmDialog() 
{
    if (dialogVisible)
    {
        return;
    }
    dialogVisible = true;

    CreateConfirmationDialog();
    gui.add(confirmDialog);
}

void ExitButton::HideConfirmDialog()
{
    if (confirmDialog) 
    {
        gui.remove(confirmDialog);
        confirmDialog = nullptr;
    }
    dialogVisible = false;
}

bool ExitButton::IsDialogVisible() const
{
    return dialogVisible;
}

void ExitButton::CreateConfirmationDialog() 
{
    // Creare dialog
    confirmDialog = tgui::ChildWindow::create();
    confirmDialog->setSize(550, 250);
    confirmDialog->setPosition(
        window.getSize().x / 2.0f - 275,
        window.getSize().y / 2.0f - 125
    );
    confirmDialog->setTitle("");
    confirmDialog->setTitleButtons(0);

    // Chenar exterior negru
    auto outerPanel = tgui::Panel::create();
    outerPanel->setSize("100%", "100%");
    outerPanel->getRenderer()->setBackgroundColor(tgui::Color::Black);
    confirmDialog->add(outerPanel);

    // Panou interior alb
    auto innerPanel = tgui::Panel::create();
    innerPanel->setSize("96%", "92%");
    innerPanel->setPosition("2%", "4%");
    innerPanel->getRenderer()->setBackgroundColor(tgui::Color::White);
    outerPanel->add(innerPanel);

    // Text "Are you sure?"
    auto questionLabel = tgui::Label::create("Are you sure?");
    questionLabel->setTextSize(32);
    questionLabel->getRenderer()->setTextColor(tgui::Color::Black);
    questionLabel->setPosition(30, 30);
    innerPanel->add(questionLabel);

    // Chenar pentru text
    auto questionBorder = tgui::Panel::create();
    questionBorder->setSize(480, 70);
    questionBorder->setPosition(20, 20);
    questionBorder->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    questionBorder->getRenderer()->setBorderColor(tgui::Color::Black);
    questionBorder->getRenderer()->setBorders({ 3, 3, 3, 3 });
    innerPanel->add(questionBorder);

    // Buton "No"
    auto noButton = tgui::Button::create("No");
    noButton->setSize(140, 50);
    noButton->setPosition(40, 140);
    noButton->getRenderer()->setBackgroundColor(tgui::Color::White);
    noButton->getRenderer()->setTextColor(tgui::Color::Black);
    noButton->getRenderer()->setBorderColor(tgui::Color::Black);
    noButton->getRenderer()->setBorders({ 3, 3, 3, 3 });
    noButton->setTextSize(24);
    noButton->onPress([this] { HideConfirmDialog(); });
    innerPanel->add(noButton);

    // Buton "Yes"
    auto yesButton = tgui::Button::create("Yes");
    yesButton->setSize(140, 50);
    yesButton->setPosition(330, 140);
    yesButton->getRenderer()->setBackgroundColor(tgui::Color::White);
    yesButton->getRenderer()->setTextColor(tgui::Color::Black);
    yesButton->getRenderer()->setBorderColor(tgui::Color::Black);
    yesButton->getRenderer()->setBorders({ 3, 3, 3, 3 });
    yesButton->setTextSize(24);
    yesButton->onPress([this] { window.close(); });
    innerPanel->add(yesButton);
}