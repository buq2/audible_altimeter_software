#include "config.hh"
#include "axlib/core/core.hh"

template<>
const char *ToString(const UiAltimeter::AltimeterUiMode en)
{
    switch(en) {
    default:
    case UiAltimeter::ALTIMETER_UI_MODE_COMPLEX:
    {
        static const char str[] = "Complex";
        return str;
    }
    case UiAltimeter::ALTIMETER_UI_MODE_FREE_FALL:
    {
        static const char str[] = "FreeF";
        return str;
    }
    }
}

template<>
const char *ToString(const axlib::DisplayBuffer::Rotation rot)
{
    switch(rot) {
    default:
    case axlib::DisplayBuffer::ROTATION_NONE:
    {
        static const char str[] = "None";
        return str;
    }
    case axlib::DisplayBuffer::ROTATION_90:
    {
        static const char str[] = "90";
        return str;
    }
    case axlib::DisplayBuffer::ROTATION_180:
    {
        static const char str[] = "180";
        return str;
    }
    case axlib::DisplayBuffer::ROTATION_270:
    {
        static const char str[] = "270";
        return str;
    }
    }
}

template<>
const char *ToString(const Config::AltitudeDisplayUnitMode en)
{
    switch(en) {
    case Config::AltitudeUnitModeMeters:
    {
        static const char str[] = "m";
        return str;
    }
    case Config::AltitudeUnitModeFeets:
    default:
    {
        static const char str[] = "ft";
        return str;
    }
    }
}

template<>
const char *ToString(const Config::SpeedDisplayUnitMode en)
{
    switch(en) {
    case Config::SpeedUnitModeMetersPerSecond:
    {
        static const char str[] = "m/s";
        return str;
    }
    case Config::SpeedUnitModeKilometersPerHour:
    {
        static const char str[] = "km/h";
        return str;
    }
    case Config::SpeedUnitModeFeetsPerSecond:
    {
        static const char str[] = "ft/s";
        return str;
    }
    default:
    case Config::SpeedUnitModeMilesPerHour:
    {
        static const char str[] = "mi/h";
        return str;
    }
    }
}

template<>
const char *ToString(const Config::TemperatureMode en)
{
    switch(en) {
    case Config::TemperatureModeCelcius:
    {
        static const char str[] = "C";
        return str;
    }
    case Config::TemperatureModeFahrenheit:
    default:
    {
        static const char str[] = "F";
        return str;
    }
    }
}

template<>
const char *ToString(const Config::FontSize en)
{
    switch(en) {
    case Config::FontSizeSmall:
    {
        static const char str[] = "Small";
        return str;
    }
    case Config::FontSizeMedium:
    {
        static const char str[] = "Medium";
        return str;
    }
    default:
    case Config::FontSizeLarge:
    {
        static const char str[] = "Large";
        return str;
    }
    }
}

template<>
const char *ToString(const Config::AltitudeAlarm::AlarmAmplitude en)
{
    switch(en) {
    case Config::AltitudeAlarm::AlarmAmplitudeWeak:
    {
        static const char str[] = "Weak";
        return str;
    }
    case Config::AltitudeAlarm::AlarmAmplitudeMedium:
    {
        static const char str[] = "Medium";
        return str;
    }
    default:
    case Config::AltitudeAlarm::AlarmAmplitudeStrong:
    {
        static const char str[] = "Strong";
        return str;
    }
    }
}

template<>
const char *ToString(const Config::AltitudeAlarm::AlarmType en)
{
    switch(en) {
    case Config::AltitudeAlarm::AlarmTypeCanopyAltitude:
    {
        static const char str[] = "Canopy";
        return str;
    }
    case Config::AltitudeAlarm::AlarmTypeFreefallBeeps:
    {
        static const char str[] = "Beeps";
        return str;
    }
    case Config::AltitudeAlarm::AlarmTypeUntilOpen:
    {
        static const char str[] = "Open";
        return str;
    }
    default:
    case Config::AltitudeAlarm::AlarmTypeLastChange:
    {
        static const char str[] = "LastChng";
        return str;
    }
    }
}

template<>
const char *ToString(const Config::DataSaveMode en)
{
    switch(en) {
    case Config::DataSaveOff:
    {
        static const char str[] = "Off";
        return str;
    }
    default:
    case Config::DataSaveAll:
    {
        static const char str[] = "All";
        return str;
    }
    }
}

#define ENUM_MACRO(T,MAXNUM) \
template<> \
T NextEnum(const T en)\
{\
    const T next = (T)(en+1);\
    if (next >= MAXNUM) {\
        return (T)0;\
    }\
    return next;\
}

ENUM_MACRO(Config::AltitudeDisplayUnitMode, Config::AltitudeUnitModeNumberOfEnums);
ENUM_MACRO(Config::SpeedDisplayUnitMode, Config::SpeedUnitModeNumberOfEnums);
ENUM_MACRO(Config::TemperatureMode, Config::TemperatureModeNumberOfEnums);
ENUM_MACRO(axlib::DisplayBuffer::Rotation, axlib::DisplayBuffer::ROTATION_NUM_ENUMS);
ENUM_MACRO(Config::FontSize, Config::FontSizeNumberOfEnums);
ENUM_MACRO(Config::AltitudeAlarm::AlarmAmplitude, Config::AltitudeAlarm::AlarmAmplitudeNumberOfEnums);
ENUM_MACRO(Config::AltitudeAlarm::AlarmType, Config::AltitudeAlarm::AlarmTypeNumberOfEnums);
ENUM_MACRO(UiAltimeter::AltimeterUiMode, UiAltimeter::ALTIMETER_UI_MODE_NUM_ENUMS);
ENUM_MACRO(Config::DataSaveMode, Config::DataSaveNumberOfEnums);

fontStyle_t *Config::GetIntChangeFont() const
{
    return &FontStyle_impact_huge;
}

uint32_t Config::GetValidMagic()
{
    return 0xbadface4;
}





uint8_t GetVolume(const Config::AltitudeAlarm::AlarmAmplitude amp)
{
    switch(amp) {
    case Config::AltitudeAlarm::AlarmAmplitudeWeak:
        return 20;
    case Config::AltitudeAlarm::AlarmAmplitudeMedium:
        return 127;
    case Config::AltitudeAlarm::AlarmAmplitudeStrong:
    default:
        return 255;
    }
}
