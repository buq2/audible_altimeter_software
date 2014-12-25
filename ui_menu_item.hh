#ifndef MENU_ITEM_HH
#define MENU_ITEM_HH

#include <string>
#include <vector>
#include <stdint.h>
#include "ui_base.hh"

class UiMenuItem
        :
        public UiBase
{
 public:
    UiMenuItem();
    UiMenuItem(UiMenuItem *parent, const std::string &label);

    virtual void Render(DisplayBuffer *buffer);
    virtual void KeyPress(const KeyCode key, const bool down);

    uint8_t GetNumberOfItems();
    UiMenuItem *GetItem(const int i);
    void AddItem(UiMenuItem *item);
    std::string GetLabel();
    uint8_t GetSelectedItemIndex();
    void Next();
    void Previous();
    UiMenuItem *GetParent();
 private:
    UiMenuItem *parent_;
    std::string label_;
    std::vector<UiMenuItem*> items_;
    uint8_t selected_sub_;
}; //class MenuItem

#endif //ifndef MENU_ITEM_HH
