#include "ui_alarms_menu.hh"


UiAlarmsMenu::UiAlarmsMenu(UiMenu *parent)
    :
      UiMenu(parent),
      free_fly_({
               UiConfigItemAltitudeAlarm(this, "Free fall 1"),
               UiConfigItemAltitudeAlarm(this, "Free fall 2"),
               UiConfigItemAltitudeAlarm(this, "Free fall 3")
               }),
      canopy_({
               UiConfigItemAltitudeAlarm(this, "Canopy 1"),
               UiConfigItemAltitudeAlarm(this, "Canopy 2"),
               UiConfigItemAltitudeAlarm(this, "Canopy 3")
               })
{
    Config *conf = GetConfig();
    free_fly_[0].SetAlarmPointer(&(conf->alarms_freefall[0]));
    free_fly_[1].SetAlarmPointer(&(conf->alarms_freefall[1]));
    free_fly_[2].SetAlarmPointer(&(conf->alarms_freefall[2]));
    canopy_[0].SetAlarmPointer(&(conf->alarms_canopy[0]));
    canopy_[1].SetAlarmPointer(&(conf->alarms_canopy[1]));
    canopy_[2].SetAlarmPointer(&(conf->alarms_canopy[2]));
}

const char *UiAlarmsMenu::GetLabel()
{
    static const char label[] = "Alarms";
    return label;
}

