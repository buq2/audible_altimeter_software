#ifndef UI_MENU_HH
#define UI_MENU_HH

#include "ui_base.hh"
#include "menu_item.hh"
#include "fonts/fontlibrary.h"

class UiMenu
        :
        public UiBase
{
 public:
    UiMenu();

    void Render(DisplayBuffer *buffer);
    void KeyPress(const UiBase::KeyCode key, const bool down);
    void Tick100ms();
 private:
    uint8_t GetNumberOfMenuRows(const DisplayBuffer &buffer);
    uint8_t GetMenuRowHeight();
 private:
    MenuItem root;
    MenuItem *current_;
    fontStyle_t *menu_font_;
}; //class UiMenu

#endif //ifdef UI_MENU_HH
