#ifndef UI_MENU_HH
#define UI_MENU_HH

#include "ui_base.hh"
#include "ui_menu_item.hh"
#include "fonts/fontlibrary.h"

class UiMenu
        :
        public UiMenuItem
{
 public:
    UiMenu();

    void Render(DisplayBuffer *buffer);
    void KeyPress(const UiBase::KeyCode key, const bool down);
 private:
    uint8_t GetNumberOfMenuRows(const DisplayBuffer &buffer);
    uint8_t GetMenuRowHeight();
 private:
    UiMenuItem *current_;
    fontStyle_t *menu_font_;
}; //class UiMenu

#endif //ifdef UI_MENU_HH
