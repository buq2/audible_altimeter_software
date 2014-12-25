#include "ui_display_menu.hh"
#include <iostream>

UiDisplayMenu::UiDisplayMenu(UiMenu *parent)
    :
      UiMenu(parent),
      altitude_(this,"Altitude unit"),
      speed_(this, "Speed unit"),
      temperature_(this, "Temp unit"),
      orientation_(this, "Disp orient"),
      font_size_(this, "Font size")
{
    Config *conf = GetConfig();
    altitude_.SetEnumPointer(&conf->altitude_unit_mode);
    speed_.SetEnumPointer(&conf->speed_unit_mode);
    temperature_.SetEnumPointer(&conf->temperature_mode);
    orientation_.SetEnumPointer(&conf->display_orientation);
    font_size_.SetEnumPointer(&conf->menu_font_size);
}

const char *UiDisplayMenu::GetLabel()
{
    static const char label[] = "Display Setup";
    return label;
}

void UiDisplayMenu::KeyPress(const UiBase::KeyCode key, const bool down)
{
    if (down) {
        switch (key) {
        case UiBase::KEY_RIGHT:
        {
            UiMenuItem *sub = GetItem(GetSelectedItemIndex());
            ((UiConfigItemEnumBase*)sub)->NextEnumValue();
            return;
        }
        }
    }

    UiMenu::KeyPress(key,down);
}

