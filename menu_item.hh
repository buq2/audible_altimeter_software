#ifndef MENU_ITEM_HH
#define MENU_ITEM_HH

#include <string>
#include <vector>

class MenuItem
{
 public:
    MenuItem();
    MenuItem(MenuItem *parent, const std::string &label);

    int GetNumberOfItems();
    MenuItem *GetItem(const int i);
    void AddItem(MenuItem *item);
    std::string GetLabel();
    int GetSelectedItemIndex();
    void Next();
    void Previous();
    MenuItem *GetParent();
 private:
    MenuItem *parent_;
    std::string label_;
    std::vector<MenuItem*> items_;
    int selected_sub_;
}; //class MenuItem

#endif //ifndef MENU_ITEM_HH
