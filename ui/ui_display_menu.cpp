#include "ui_display_menu.hh"

UiDisplayMenu::UiDisplayMenu(UiMenu *parent)
    :
      UiMenu(parent),
      altitude_(this,"Altitude unit"),
      speed_(this, "Speed unit"),
      temperature_(this, "Temp unit"),
      orientation_(this, "Disp orient"),
      font_size_(this, "Font size"),
      altimeter_default_mode_(this, "Def mode"),
      altimeter_freefall_mode_(this, "Freef mode"),
      altimeter_canopy_mode_(this,"Canopy mode"),
      altimeter_climb_mode_(this,"Plane mode")
{
    Config *conf = GetConfig();
    altitude_.SetEnumPointer(&conf->altitude_unit_mode);
    speed_.SetEnumPointer(&conf->speed_unit_mode);
    temperature_.SetEnumPointer(&conf->temperature_mode);
    orientation_.SetEnumPointer(&conf->display_orientation);
    font_size_.SetEnumPointer(&conf->menu_font_size);
    altimeter_default_mode_.SetEnumPointer(&conf->default_altimeter_ui_mode);
    altimeter_freefall_mode_.SetEnumPointer(&conf->freefall_altimeter_ui_mode);
    altimeter_canopy_mode_.SetEnumPointer(&conf->canopy_altimeter_ui_mode);
    altimeter_climb_mode_.SetEnumPointer(&conf->climb_altimeter_ui_mode);
}

const char *UiDisplayMenu::GetLabel()
{
    static const char label[] = "Display Setup";
    return label;
}

