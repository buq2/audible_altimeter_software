#ifndef UI_CONFIG_ITEM_ENUM_HH
#define UI_CONFIG_ITEM_ENUM_HH

#include "ui_menu_item.hh"

template <typename T>
class UiConfigItemEnum
        :
        public UiMenuItem
{
 public:
    UiConfigItemEnum(UiMenu *parent, const char *label)
        :
          UiMenuItem(parent),
          label_(label)
    {
    }

    void SetEnumPointer(T *ptr)
    {
        ptr_ = ptr;
    }

    const char *GetLabel()
    {
        return label_;
    }

    const char *GetLabelValue()
    {
        return ToString(*ptr_);
    }
 private:
    T *ptr_;
    const char *label_;
}; //class UiConfigItemEnum

#endif //ifndef UI_CONFIG_ITEM_ENUM_HH
