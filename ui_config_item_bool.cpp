#include "ui_config_item_bool.hh"
#include "ui_menu.hh"

UiConfigItemBool::UiConfigItemBool(UiMenu *parent, const char *label)
    :
      UiMenuItem(parent),
      label_(label)
{
}

const char *UiConfigItemBool::GetLabelValue()
{
    if (*ptr_) {
        static const char str[] = "true";
        return str;
    } else {
        static const char str[] = "false";
        return str;
    }
}

void UiConfigItemBool::SetPointer(bool *ptr)
{
    ptr_ = ptr;
}

const char *UiConfigItemBool::GetLabel()
{
    return label_;
}

void UiConfigItemBool::Activated()
{
    *ptr_ = !*ptr_;
    ((UiMenu*)GetParent())->Activate();
}
