#ifndef UI_ALARMS_MENU
#define UI_ALARMS_MENU

#include "ui_menu.hh"
#include "ui_config_item_altitude_alarm.hh"

class UiAlarmsMenu
        :
        public UiMenu
{
 public:
    UiAlarmsMenu(UiMenu *parent);
    const char *GetLabel();
 private:
    UiConfigItemAltitudeAlarm free_fly_[3];
    UiConfigItemAltitudeAlarm canopy_[3];
}; //class UiAlarmsMenu

#endif //ifndef UI_ALARMS_MENU
