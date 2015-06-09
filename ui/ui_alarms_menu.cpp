#include "ui_alarms_menu.hh"


UiAlarmsMenu::UiAlarmsMenu(UiMenu *parent)
    :
      UiMenu(parent),
      freefall_({
               UiConfigItemAltitudeAlarm(this, "Freefall 1"),
               UiConfigItemAltitudeAlarm(this, "Freefall 2"),
               UiConfigItemAltitudeAlarm(this, "Freefall 3")
               }),
      canopy_({
               UiConfigItemAltitudeAlarm(this, "Canopy 1"),
               UiConfigItemAltitudeAlarm(this, "Canopy 2"),
               UiConfigItemAltitudeAlarm(this, "Canopy 3")
               }),
      options_({
               UiConfigItemBeepOption(this,"Beep freefall"),
               UiConfigItemBeepOption(this,"Beep clmb alt"),
               UiConfigItemBeepOption(this,"Beep ground")
               })
{
    Config *conf = GetConfig();
    freefall_[0].SetAlarmPointer(&(conf->beeper.alarms_freefall[0]));
    freefall_[1].SetAlarmPointer(&(conf->beeper.alarms_freefall[1]));
    freefall_[2].SetAlarmPointer(&(conf->beeper.alarms_freefall[2]));
    canopy_[0].SetAlarmPointer(&(conf->beeper.alarms_canopy[0]));
    canopy_[1].SetAlarmPointer(&(conf->beeper.alarms_canopy[1]));
    canopy_[2].SetAlarmPointer(&(conf->beeper.alarms_canopy[2]));
    options_[0].SetOptionPointer(&(conf->beeper.at_freefall));
    options_[1].SetOptionPointer(&(conf->beeper.climb_altitude));
    options_[2].SetOptionPointer(&(conf->beeper.at_ground));
}

const char *UiAlarmsMenu::GetLabel()
{
    static const char label[] = "Alarms";
    return label;
}

