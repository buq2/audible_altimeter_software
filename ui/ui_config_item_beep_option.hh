#ifndef UI_CONFIG_ITEM_BEEP_OPTION_HH
#define UI_CONFIG_ITEM_BEEP_OPTION_HH

#include "ui_menu.hh"
#include "config.hh"
#include "ui_config_item_int.hh"
#include "ui_config_item_enum.hh"
#include "ui_config_item_bool.hh"

class UiConfigItemBeepOption
        :
        public UiMenu
{
 public:
    UiConfigItemBeepOption(UiMenu *parent, const char *label);
    void SetOptionPointer(Config::Beeper::BeepOption *alarm);
    const char *GetLabel();
    const char *GetLabelValue();
 private:
    UiConfigItemEnum<Config::AltitudeAlarm::AlarmAmplitude> alarm_amplitude_;
    UiConfigItemBool enabled_;
    const char *label_;
    Config::Beeper::BeepOption *alarm_;
}; //class UiConfigItemBeepOption

#endif //ifndef UI_CONFIG_ITEM_BEEP_OPTION_HH
