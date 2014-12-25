#ifndef CONFIG_HH
#define CONFIG_HH

#include <stdint.h>

struct Config
{
    typedef enum
    {
        AltitudeUnitModeMeters = 0,
        AltitudeUnitModeFeets = 1,
        AltitudeUnitModeNumberOfEnums = 2
    } AltitudeDisplayUnitMode;

    typedef enum
    {
        SpeedUnitModeMetersPerSecond = 0,
        SpeedUnitModeKilometersPerHour = 1,
        SpeedUnitModeFeetsPerSecond = 2,
        SpeedUnitModeMilesPerHour = 3,
        SpeedUnitModeNumberOfEnums = 4
    } SpeedDisplayUnitMode;

    typedef enum
    {
        TemperatureModeCelcius = 0,
        TemperatureModeFahrenheit = 1,
        TemperatureModeNumberOfEnums = 2
    } TemperatureMode;

    typedef enum
    {
        DisplayOrientationNormal = 0,
        DisplayOrientation180Rotated = 1,
        DisplayOrientationNumberOfEnums = 2
    } DisplayOrientation;

    typedef enum
    {
        FontSizeSmall = 0,
        FontSizeMedium = 1,
        FontSizeLarge = 2,
        FontSizeNumberOfEnums = 3
    } FontSize;

    struct AltitudeAlarm
    {
        AltitudeAlarm()
            :
              altitude(4000),
              amplitude(127),
              enabled(false)
        {
        }

        int16_t altitude;
        uint8_t amplitude;
        bool enabled;
    };

    struct Time
    {
        Time()
            :
              hour(0),
              minute(0),
              second(0)
        {
        }

        uint8_t hour;
        uint8_t minute;
        uint8_t second;
    };

    struct Date
    {
        Date()
            :
              year(2014),
              month(1),
              day(1)
        {
        }

        uint16_t year;
        uint8_t month;
        uint8_t day;
    };

    struct JumpLogInfo
    {
        struct JumpTime
        {
            JumpTime()
                :
                  hours(0),
                  minutes(0),
                  seconds(0)
            {
            }

            uint16_t hours;
            uint8_t minutes;
            uint8_t seconds;
        };

        JumpTime free_fall_time;
        JumpTime canopy_time;
        uint16_t next_jump_number;
    };

    struct SleepAlarm
    {
        SleepAlarm()
            :
              enabled(false)
        {
        }
        Time alarm_time;
        bool enabled;
    };

    Config()
        :
          altitude_unit_mode(AltitudeUnitModeMeters),
          speed_unit_mode(SpeedUnitModeMetersPerSecond),
          temperature_mode(TemperatureModeCelcius),
          display_orientation(DisplayOrientationNormal),
          menu_font_size(FontSizeMedium)
    {
    }

    Time time;
    Date date;
    AltitudeDisplayUnitMode altitude_unit_mode;
    SpeedDisplayUnitMode speed_unit_mode;
    TemperatureMode temperature_mode;
    DisplayOrientation display_orientation;
    AltitudeAlarm alarms_freefall[3];
    AltitudeAlarm alarms_canopy[3];
    FontSize menu_font_size;
}; //struct Config

template<typename T>
const char *ToString(const T en)
{
    static const char str[] = "<error>";
    return str;
}

template<>
const char *ToString(const Config::AltitudeDisplayUnitMode en);

template<>
const char *ToString(const Config::SpeedDisplayUnitMode en);

template<>
const char *ToString(const Config::TemperatureMode en);

template<>
const char *ToString(const Config::DisplayOrientation en);

template<>
const char *ToString(const Config::AltitudeAlarm en);

template<>
const char *ToString(const Config::FontSize en);

template<typename T>
T NextEnum(const T en)
{
    return en;
}

template<>
Config::AltitudeDisplayUnitMode NextEnum(const Config::AltitudeDisplayUnitMode en);

template<>
Config::SpeedDisplayUnitMode NextEnum(const Config::SpeedDisplayUnitMode en);

template<>
Config::TemperatureMode NextEnum(const Config::TemperatureMode en);

template<>
Config::DisplayOrientation NextEnum(const Config::DisplayOrientation en);

template<>
Config::AltitudeAlarm NextEnum(const Config::AltitudeAlarm en);

template<>
Config::FontSize NextEnum(const Config::FontSize en);

#endif //ifndef CONFIG_HH
