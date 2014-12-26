#ifndef UI_CONFIG_ITEM_ALTITUDE_ALARM_HH
#define UI_CONFIG_ITEM_ALTITUDE_ALARM_HH

#include "ui_menu.hh"
#include "config.hh"
#include "ui_config_item_int.hh"
#include "ui_config_item_enum.hh"
#include "ui_config_item_bool.hh"

class UiConfigItemAltitudeAlarm
        :
        public UiMenu
{
 public:
    UiConfigItemAltitudeAlarm(UiMenu *parent, const char *label);
    void SetAlarmPointer(Config::AltitudeAlarm *alarm);
    const char *GetLabel();
    const char *GetLabelValue();
 private:
    UiConfigItemInt<int16_t> altitude_;
    UiConfigItemEnum<Config::AltitudeAlarm::AlarmAmplitude> alarm_amplitude_;
    UiConfigItemBool enabled_;
    const char *label_;
    Config::AltitudeAlarm *alarm_;
}; //class UiConfigItemAltitudeAlarm

#endif //ifndef UI_CONFIG_ITEM_ALTITUDE_ALARM_HH
