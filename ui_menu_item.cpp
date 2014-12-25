#include "ui_menu_item.hh"

UiMenuItem::UiMenuItem()
    :
    parent_(NULL),
    selected_sub_(0)
{
}

UiMenuItem::UiMenuItem(UiMenuItem *parent, const std::string &label)
    :
    parent_(parent),
    label_(label),
    selected_sub_(0)
{
    parent_->AddItem(this);
}

void UiMenuItem::Render(DisplayBuffer *buffer)
{

}

void UiMenuItem::KeyPress(const UiBase::KeyCode key, const bool down)
{

}

uint8_t UiMenuItem::GetNumberOfItems()
{
    return items_.size();
}

UiMenuItem *UiMenuItem::GetItem(const int i)
{
    return items_[i];
}

void UiMenuItem::AddItem(UiMenuItem *item)
{
    items_.push_back(item);
}

std::string UiMenuItem::GetLabel()
{
    return label_;
}

uint8_t UiMenuItem::GetSelectedItemIndex()
{
    return selected_sub_;
}

void UiMenuItem::Next()
{
    selected_sub_++;
    selected_sub_ = std::min(selected_sub_,(uint8_t)(GetNumberOfItems()-1));
}

void UiMenuItem::Previous()
{
    if (selected_sub_ == 0) {
        return;
    }
    --selected_sub_;
}

UiMenuItem *UiMenuItem::GetParent()
{
    return parent_;
}
