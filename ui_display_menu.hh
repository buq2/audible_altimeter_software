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
    virtual void KeyPress(const UiBase::KeyCode key, const bool down);
 private:
    UiConfigItemEnum<Config::AltitudeDisplayUnitMode> altitude_;
    UiConfigItemEnum<Config::SpeedDisplayUnitMode> speed_;
    UiConfigItemEnum<Config::TemperatureMode> temperature_;
    UiConfigItemEnum<Config::DisplayOrientation> orientation_;
    UiConfigItemEnum<Config::FontSize> font_size_;
}; //class UiDisplayMenu

#endif //ifndef UI_DISPLAY_MENU
