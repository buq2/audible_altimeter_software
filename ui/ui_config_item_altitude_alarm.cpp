#include "ui_config_item_altitude_alarm.hh"

UiConfigItemAltitudeAlarm::UiConfigItemAltitudeAlarm(UiMenu *parent, const char *label)
    :
      UiMenu(parent),
      altitude_(this, "Altitude",0, 7000),
      alarm_amplitude_(this, "Volume"),
      alarm_type_(this, "Type"),
      enabled_(this, "Enabled"),
      label_(label)
{

}

void UiConfigItemAltitudeAlarm::SetAlarmPointer(Config::AltitudeAlarm *alarm)
{
    altitude_.SetPointer(&(alarm->altitude));
    alarm_amplitude_.SetEnumPointer(&(alarm->amplitude));
    enabled_.SetPointer(&(alarm->enabled));
    alarm_type_.SetEnumPointer(&alarm->type);
    alarm_ = alarm;
}

const char *UiConfigItemAltitudeAlarm::GetLabel()
{
    return label_;
}

const char *UiConfigItemAltitudeAlarm::GetLabelValue()
{
    if (alarm_->enabled) {
        return altitude_.GetLabelValue();
    } else {
        static const char str[] = "off";
        return str;
    }
}
