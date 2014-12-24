#ifndef CONFIG_HH
#define CONFIG_HH

#include <stdint.h>

struct Config
{
    typedef enum
    {
        AltitudeUnitModeMeters,
        AltitudeUnitModeFeets,
    } AltitudeDisplayUnitMode;

    typedef enum
    {
        SpeedUnitModeMetersPerSecond,
        SpeedUnitModeKilometersPerHour,
        SpeedUnitModeFeetsPerSecond,
        SpeedUnitModeMilesPerHour,
    } SpeedDisplayUnitMode;

    typedef enum
    {
        TemperatureModeCelcius,
        TemperatureModeFahrenheit
    } TemperatureMode;

    typedef enum
    {
        DisplayOrientationNormal,
        DisplayOrientation180Rotated
    } DisplayOrientation;

    typedef enum
    {
        FontSizeSmall,
        FontSizeMedium,
        FontSizeLarge,
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

#endif //ifndef CONFIG_HH
