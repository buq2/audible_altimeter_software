#ifndef MENU_ITEM_HH
#define MENU_ITEM_HH

#include <string>
#include <vector>
#include <stdint.h>

class MenuItem
{
 public:
    MenuItem();
    MenuItem(MenuItem *parent, const std::string &label);

    uint8_t GetNumberOfItems();
    MenuItem *GetItem(const int i);
    void AddItem(MenuItem *item);
    std::string GetLabel();
    uint8_t GetSelectedItemIndex();
    void Next();
    void Previous();
    MenuItem *GetParent();
 private:
    MenuItem *parent_;
    std::string label_;
    std::vector<MenuItem*> items_;
    uint8_t selected_sub_;
}; //class MenuItem

#endif //ifndef MENU_ITEM_HH
