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
    UiMenu(UiMenu *parent);

    virtual void Render(DisplayBuffer *buffer);
    virtual void KeyPress(const UiBase::KeyCode key, const bool down);

    uint8_t GetNumberOfItems();
    UiMenuItem *GetItem(const uint8_t i);
    void AddItem(UiMenuItem *item);
    uint8_t GetSelectedItemIndex();

    void Next();
    void Previous();
    void Activate();
 private:
    uint8_t GetNumberOfMenuRows(const DisplayBuffer &buffer);
    uint8_t GetMenuRowHeight();
 private:
    UiMenuItem *active_item_;
    fontStyle_t *menu_font_;
    std::vector<UiMenuItem*> items_;
    uint8_t selected_sub_;
}; //class UiMenu

#endif //ifdef UI_MENU_HH
