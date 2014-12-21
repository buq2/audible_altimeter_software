#include "ui_menu.hh"
#include <iostream>

UiMenu::UiMenu()
{
    MenuItem *m1 = new MenuItem(&root, "item1");
    MenuItem *m2 = new MenuItem(&root, "item2");
    MenuItem *m3 = new MenuItem(&root, "item3");
    MenuItem *m4 = new MenuItem(&root, "item4");

    new MenuItem(m1, "item1-sub1");
    new MenuItem(m1, "item1-sub2");
    new MenuItem(m1, "item1-sub3");

    new MenuItem(m2, "item2-abc");
    new MenuItem(m2, "item2-cde");
    new MenuItem(m2, "item2-efg");

    new MenuItem(m3, "item3-a");
    new MenuItem(m3, "item3-b");

    new MenuItem(m4, "item4-a");

    current_ = &root;
}

void UiMenu::Render(DisplayBuffer *buffer)
{


    std::cout << "---------" << std::endl;
    for (int i = 0; i < current_->GetNumberOfItems(); ++i) {

        if (i == current_->GetSelectedItemIndex()) {
            std::cout << "=";
        }

        MenuItem *item = current_->GetItem(i);
        std::cout << item->GetLabel() << std::endl;
    }
}

void UiMenu::KeyPress(const UiBase::KeyCode key, const bool down)
{
    if (!down) {
        return;
    }

    switch(key) {
    case UiBase::KEY_UP:
        current_->Previous();
        break;
    case UiBase::KEY_DOWN:
        current_->Next();
        break;
    case UiBase::KEY_LEFT:
    {
        MenuItem *parent = current_->GetParent();
        if (NULL != parent) {
            current_ = parent;
        }
        break;
    }
    case UiBase::KEY_RIGHT:
    {
        MenuItem *sub = current_->GetItem(current_->GetSelectedItemIndex());
        if (NULL != sub) {
            current_ = sub;
        }
        break;
    }
    }
}

void UiMenu::Tick100ms()
{

}
