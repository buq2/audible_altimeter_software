#ifndef UI_MENU_HH
#define UI_MENU_HH

#include "ui_base.hh"
#include "menu_item.hh"

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
    MenuItem root;
    MenuItem *current_;
}; //class UiMenu

#endif //ifdef UI_MENU_HH
