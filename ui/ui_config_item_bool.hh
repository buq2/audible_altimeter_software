#ifndef UI_CONFIG_ITEM_BOOL_HH
#define UI_CONFIG_ITEM_BOOL_HH

#include "ui_menu_item.hh"

class UiConfigItemBool
        :
        public UiMenuItem
{
 public:
    UiConfigItemBool(UiMenu *parent, const char *label);
    const char *GetLabelValue();
    void SetPointer(bool *ptr);
    const char *GetLabel();
    void Activated();
 private:
    bool *ptr_;
    const char *label_;
};

#endif //ifndef UI_CONFIG_ITEM_BOOL_HH
