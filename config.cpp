#include "config.hh"

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
const char *ToString(const Config::DisplayOrientation en)
{
    switch(en) {
    case Config::DisplayOrientationNormal:
    {
        static const char str[] = "Normal";
        return str;
    }
    case Config::DisplayOrientation180Rotated:
    default:
    {
        static const char str[] = "Rotated";
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

#define ENUM_MACRO(T,MAXNUM) \
template<> \
T NextEnum(const T en)\
{\
    const T next = (T)(en+1);\
    if (en == MAXNUM) {\
        return (T)0;\
    }\
    return next;\
}

ENUM_MACRO(Config::AltitudeDisplayUnitMode, Config::AltitudeUnitModeNumberOfEnums);
ENUM_MACRO(Config::SpeedDisplayUnitMode, Config::SpeedUnitModeNumberOfEnums);
ENUM_MACRO(Config::TemperatureMode, Config::TemperatureModeNumberOfEnums);
ENUM_MACRO(Config::DisplayOrientation, Config::DisplayOrientationNumberOfEnums);
ENUM_MACRO(Config::FontSize, Config::FontSizeNumberOfEnums);
