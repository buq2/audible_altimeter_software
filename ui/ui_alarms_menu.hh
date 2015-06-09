#ifndef UI_ALARMS_MENU
#define UI_ALARMS_MENU

#include "ui_menu.hh"
#include "ui_config_item_altitude_alarm.hh"
#include "ui_config_item_beep_option.hh"

class UiAlarmsMenu
        :
        public UiMenu
{
 public:
    UiAlarmsMenu(UiMenu *parent);
    const char *GetLabel();
 private:
    UiConfigItemAltitudeAlarm freefall_[3];
    UiConfigItemAltitudeAlarm canopy_[3];
    UiConfigItemBeepOption options_[5];
}; //class UiAlarmsMenu

#endif //ifndef UI_ALARMS_MENU
