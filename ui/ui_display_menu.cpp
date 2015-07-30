#include "ui_display_menu.hh"

UiDisplayMenu::UiDisplayMenu(UiMenu *parent)
    :
      UiMenu(parent),
      altitude_(this,"Alt unit"),
      altitude_rounding_(this,"Round md"),
      speed_(this, "Spd unit"),
      temperature_(this, "Tmp unit"),
      orientation_(this, "Disp ori"),
      font_size_(this, "Font sz"),
      altimeter_default_mode_(this, "Def md"),
      altimeter_freefall_mode_(this, "Free md"),
      altimeter_canopy_mode_(this,"Cnpy md"),
      altimeter_climb_mode_(this,"Pln md"),
      invert_colors_(this,"Inv clrs")
{
    Config *conf = GetConfig();
    altitude_.SetEnumPointer(&conf->altitude_unit_mode);
    altitude_rounding_.SetEnumPointer(&conf->display_round_mode);
    speed_.SetEnumPointer(&conf->speed_unit_mode);
    temperature_.SetEnumPointer(&conf->temperature_mode);
    orientation_.SetEnumPointer(&conf->display_orientation);
    font_size_.SetEnumPointer(&conf->menu_font_size);
    altimeter_default_mode_.SetEnumPointer(&conf->default_altimeter_ui_mode);
    altimeter_freefall_mode_.SetEnumPointer(&conf->freefall_altimeter_ui_mode);
    altimeter_canopy_mode_.SetEnumPointer(&conf->canopy_altimeter_ui_mode);
    altimeter_climb_mode_.SetEnumPointer(&conf->climb_altimeter_ui_mode);
    invert_colors_.SetPointer(&conf->invert_colors);
}

const char *UiDisplayMenu::GetLabel()
{
    static const char label[] = "Display Setup";
    return label;
}

