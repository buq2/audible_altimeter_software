#include "ui_config_item_beep_option.hh"

UiConfigItemBeepOption::UiConfigItemBeepOption(UiMenu *parent, const char *label)
    :
      UiMenu(parent),
      alarm_amplitude_(this, "Alarm volume"),
      enabled_(this, "Enabled"),
      label_(label)
{

}

void UiConfigItemBeepOption::SetOptionPointer(Config::Beeper::BeepOption *alarm)
{
    alarm_amplitude_.SetEnumPointer(&(alarm->amplitude));
    enabled_.SetPointer(&(alarm->enabled));
    alarm_ = alarm;
}

const char *UiConfigItemBeepOption::GetLabel()
{
    return label_;
}

const char *UiConfigItemBeepOption::GetLabelValue()
{
    if (alarm_->enabled) {
        static const char str[] = "on";
        return str;
    } else {
        static const char str[] = "off";
        return str;
    }
}
