#ifndef UI_DISPLAY_MENU
#define UI_DISPLAY_MENU

#include "ui_menu.hh"
#include "ui_config_item_enum.hh"

class UiDisplayMenu
        :
        public UiMenu
{
 public:
    UiDisplayMenu(UiMenu *parent);
    const char *GetLabel();
 private:
    UiConfigItemEnum<Config::AltitudeDisplayUnitMode> altitude_;
    UiConfigItemEnum<Config::SpeedDisplayUnitMode> speed_;
    UiConfigItemEnum<Config::TemperatureMode> temperature_;
    UiConfigItemEnum<axlib::DisplayBuffer::Rotation> orientation_;
    UiConfigItemEnum<Config::FontSize> font_size_;
    UiConfigItemEnum<UiAltimeter::AltimeterUiMode> altimeter_default_mode_;
    UiConfigItemEnum<UiAltimeter::AltimeterUiMode> altimeter_freefall_mode_;
    UiConfigItemEnum<UiAltimeter::AltimeterUiMode> altimeter_canopy_mode_;
    UiConfigItemEnum<UiAltimeter::AltimeterUiMode> altimeter_climb_mode_;
}; //class UiDisplayMenu

#endif //ifndef UI_DISPLAY_MENU
