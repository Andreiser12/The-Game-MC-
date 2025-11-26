#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>


class ExitButton
{
private:
    sf::RenderWindow& window;
    tgui::Gui& gui;
    tgui::Button::Ptr exitButton;
    tgui::ChildWindow::Ptr confirmDialog;
    bool dialogVisible;

private:
    void SetupExitButton(float x, float y, float width, float height);
    void CreateConfirmationDialog();

public:
    ExitButton(sf::RenderWindow& win, tgui::Gui& g);

    void Create(float x, float y, float width = 200, float height = 50);
    void ShowConfirmDialog();
    void HideConfirmDialog();
    bool IsDialogVisible() const;


};

