#ifndef MENU_ITEM_HH
#define MENU_ITEM_HH

#include <stdint.h>
#include "ui_base.hh"
#include "config.hh"

class UiMenu;

class UiMenuItem
        :
        public UiBase
{
 public:
    UiMenuItem();
    UiMenuItem(UiMenu *parent);

    virtual void Render(axlib::DisplayBuffer *buffer);
    virtual void KeyPress(const KeyCode key, const bool down);

    virtual const char *GetLabel();
    virtual const char *GetLabelValue();

    UiMenuItem *GetParent();
    virtual Config *GetConfig();
    virtual MiscInformation *GetMiscInformation();
    virtual void Activated();
 protected:
 private:
    UiMenu *parent_;
}; //class MenuItem

#endif //ifndef MENU_ITEM_HH
