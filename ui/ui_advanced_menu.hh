#ifndef UI_ADVANCED_MENU
#define UI_ADVANCED_MENU

#include "ui_menu.hh"
#include "ui_config_item_enum.hh"

class UiAdvancedMenu
        :
        public UiMenu
{
 public:
    UiAdvancedMenu(UiMenu *parent);
    const char *GetLabel();
 private:
    UiConfigItemEnum<Config::AltitudeSensorOversampleRate> oversample_rate_;
}; //class UiAdvancedMenu

#endif //ifndef UI_ADVANCED_MENU
