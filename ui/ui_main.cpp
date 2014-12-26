#include "ui_main.hh"
#include <iostream>

UiMain::UiMain(Config *config, Sensors *sensors)
    :
      altimeter_(sensors),
      menu_(config),
      ui_selection_(UI_SELECTION_MENU)
{

}

void UiMain::Render(DisplayBuffer *buffer)
{
    switch(ui_selection_) {
    case UI_SELECTION_MENU:
        menu_.Render(buffer);
        break;
    case UI_SELECTION_ALTIMETER:
    default:
        altimeter_.Render(buffer);
        break;
    }
}

void UiMain::KeyPress(const UiBase::KeyCode key, const bool down)
{    
    switch(ui_selection_) {
    case UI_SELECTION_MENU:
        if (down && key == KEY_LEFT && menu_.IsAtMainMenu()) {
            ui_selection_ = UI_SELECTION_ALTIMETER;
            return;
        }
        menu_.KeyPress(key,down);
        break;
    case UI_SELECTION_ALTIMETER:
    default:
        if (down && key == KEY_LEFT) {
            ui_selection_ = UI_SELECTION_MENU;
            return;
        }
        altimeter_.KeyPress(key,down);
        break;
    }
}

void UiMain::Tick100ms()
{
    altimeter_.Tick100ms();
    menu_.Tick100ms();
}