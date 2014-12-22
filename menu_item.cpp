#include "menu_item.hh"

MenuItem::MenuItem()
    :
    parent_(NULL),
    selected_sub_(0)
{
}

MenuItem::MenuItem(MenuItem *parent, const std::string &label)
    :
    parent_(parent),
    label_(label),
    selected_sub_(0)
{
    parent_->AddItem(this);
}

uint8_t MenuItem::GetNumberOfItems()
{
    return items_.size();
}

MenuItem *MenuItem::GetItem(const int i)
{
    return items_[i];
}

void MenuItem::AddItem(MenuItem *item)
{
    items_.push_back(item);
}

std::string MenuItem::GetLabel()
{
    return label_;
}

uint8_t MenuItem::GetSelectedItemIndex()
{
    return selected_sub_;
}

void MenuItem::Next()
{
    selected_sub_++;
    selected_sub_ = std::min(selected_sub_,(uint8_t)(GetNumberOfItems()-1));
}

void MenuItem::Previous()
{
    if (selected_sub_ == 0) {
        return;
    }
    --selected_sub_;
}

MenuItem *MenuItem::GetParent()
{
    return parent_;
}
