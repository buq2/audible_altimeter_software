#ifndef UI_CONFIG_ITEM_ENUM_HH
#define UI_CONFIG_ITEM_ENUM_HH

#include "ui_menu_item.hh"
#include "config.hh"
#include "ui_menu.hh"

class UiConfigItemEnumBase
        :
        public UiMenuItem
{
 public:
    UiConfigItemEnumBase(UiMenu *parent);
    virtual void NextEnumValue() = 0;
};

template <typename T>
class UiConfigItemEnum
        :
        public UiConfigItemEnumBase
{
 public:
    UiConfigItemEnum(UiMenu *parent, const char *label)
        :
          UiConfigItemEnumBase(parent),
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

    void NextEnumValue()
    {
        *ptr_ = NextEnum(*ptr_);
    }

    void Activated()
    {
        NextEnumValue();
        ((UiMenu*)GetParent())->Activate();
    }
 private:
    T *ptr_;
    const char *label_;
}; //class UiConfigItemEnum

#endif //ifndef UI_CONFIG_ITEM_ENUM_HH
