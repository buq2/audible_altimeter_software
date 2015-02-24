#include "ui_menu_item.hh"
#include "ui_menu.hh"
#include "common.hh"

UiMenuItem::UiMenuItem()
    :
    parent_(NULL)
{
}

UiMenuItem::UiMenuItem(UiMenu *parent)
    :
    parent_(parent)
{
    parent_->AddItem(this);
}

void UiMenuItem::Render(axlib::DisplayBuffer */*buffer*/)
{
}

void UiMenuItem::KeyPress(const UiBase::KeyCode /*key*/, const bool down)
{
    if (!down) {
        return;
    }

    UiMenuItem *parent = GetParent();
    if (NULL == parent) {
        return;
    }
    ((UiMenu*)parent)->Activate();
}

const char *UiMenuItem::GetLabel()
{
    static const char label[] = "";
    return label;
}

const char *UiMenuItem::GetLabelValue()
{
    static const char label_value[] = "";
    return label_value;
}

UiMenuItem *UiMenuItem::GetParent()
{
    return parent_;
}

Config *UiMenuItem::GetConfig()
{
    return parent_->GetConfig();
}

void UiMenuItem::Activated()
{

}
