#ifndef CONFIG_HH
#define CONFIG_HH

#include <stdint.h>
#include "axlib/displays/fonts/fontlibrary.h"
#include "axlib/displays/display_buffer.hh"
#include "ui/ui_altimeter.hh"

struct Config
{
    fontStyle_t *GetIntChangeFont() const;

    typedef enum
    {
        AltitudeSensorOversampleRate1 = 0,
        AltitudeSensorOversampleRate2 = 1,
        AltitudeSensorOversampleRate4 = 2,
        AltitudeSensorOversampleRate8 = 3,
        AltitudeSensorOversampleRate16 = 4,
        AltitudeSensorOversampleRate32 = 5,
        AltitudeSensorOversampleRate64 = 6,
        AltitudeSensorOversampleRate128 = 7,
        AltitudeSensorOversampleRateNumberOfEnums = 8
    } AltitudeSensorOversampleRate;

    typedef enum
    {
        AltitudeUnitModeMeters = 0,
        AltitudeUnitModeFeets = 1,
        AltitudeUnitModeNumberOfEnums = 2
    } AltitudeDisplayUnitMode;

    typedef enum
    {
        AltitudeDisplayRoundModeNone = 0,
        AltitudeDisplayRoundMode1 = 1,
        AltitudeDisplayRoundMode5 = 2,
        AltitudeDisplayRoundMode10 = 3,
        AltitudeDisplayRoundModeNumberOfEnums = 4
    } AltitudeDisplayRoundMode;

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
        FontSizeSmall = 0,
        FontSizeMedium = 1,
        FontSizeLarge = 2,
        FontSizeNumberOfEnums = 3
    } FontSize;

    typedef enum
    {
        DataSaveOff = 0,
        DataSaveAll = 1,
        DataSaveNumberOfEnums = 2
    } DataSaveMode;

    struct AltitudeAlarm
    {
        typedef enum
        {
            AlarmAmplitudeWeak = 0,
            AlarmAmplitudeMedium = 1,
            AlarmAmplitudeStrong = 2,
            AlarmAmplitudeNumberOfEnums = 3
        } AlarmAmplitude;

        typedef enum
        {
            AlarmTypeFreefallBeeps = 0,
            AlarmTypeUntilOpen = 1,
            AlarmTypeLastChange = 2,
            AlarmTypeCanopyAltitude = 3,
            AlarmTypeNumberOfEnums = 4,
        } AlarmType;

        AltitudeAlarm()
            :
              altitude(2000),
              amplitude(AlarmAmplitudeMedium),
              enabled(false),
              type(AlarmTypeFreefallBeeps)
        {
        }

        int16_t altitude;
        AlarmAmplitude amplitude;
        bool enabled;
        AlarmType type;
    };

    struct Beeper
    {
        struct BeepOption
        {
            BeepOption()
                :
                  amplitude(AltitudeAlarm::AlarmAmplitudeMedium),
                  enabled(true)
            {
            }

            AltitudeAlarm::AlarmAmplitude amplitude;
            bool enabled;
        };

        AltitudeAlarm alarms_freefall[3];
        AltitudeAlarm alarms_canopy[3];
        BeepOption at_plane;
        BeepOption climb_altitude;
        BeepOption at_freefall;
        BeepOption at_canopy;
        BeepOption at_ground;
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
          magic_number(GetValidMagic()),
          altitude_unit_mode(AltitudeUnitModeMeters),
          speed_unit_mode(SpeedUnitModeMetersPerSecond),
          temperature_mode(TemperatureModeCelcius),
          display_orientation(axlib::DisplayBuffer::ROTATION_NONE),
          menu_font_size(FontSizeMedium),
          default_altimeter_ui_mode(UiAltimeter::ALTIMETER_UI_MODE_COMPLEX),
          freefall_altimeter_ui_mode(UiAltimeter::ALTIMETER_UI_MODE_FREE_FALL),
          canopy_altimeter_ui_mode(UiAltimeter::ALTIMETER_UI_MODE_COMPLEX),
          climb_altimeter_ui_mode(UiAltimeter::ALTIMETER_UI_MODE_COMPLEX),
          log_save_mode(DataSaveOff),
          display_round_mode(AltitudeDisplayRoundMode5),
          oversample_rate(AltitudeSensorOversampleRate32)
    {
    }

    static bool IsValidConfig(const Config &conf)
    {
        return conf.magic_number == GetValidMagic();
    }

    static uint32_t GetValidMagic();

    // Magic number is different for each version of config
    uint32_t magic_number;
    Time time;
    Date date;
    AltitudeDisplayUnitMode altitude_unit_mode;
    SpeedDisplayUnitMode speed_unit_mode;
    TemperatureMode temperature_mode;
    axlib::DisplayBuffer::Rotation display_orientation;
    Beeper beeper;
    FontSize menu_font_size;
    UiAltimeter::AltimeterUiMode default_altimeter_ui_mode;
    UiAltimeter::AltimeterUiMode freefall_altimeter_ui_mode;
    UiAltimeter::AltimeterUiMode canopy_altimeter_ui_mode;
    UiAltimeter::AltimeterUiMode climb_altimeter_ui_mode;
    DataSaveMode log_save_mode;
    AltitudeDisplayRoundMode display_round_mode;
    bool invert_colors;
    AltitudeSensorOversampleRate oversample_rate;
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
const char *ToString(const axlib::DisplayBuffer::Rotation rot);

template<>
const char *ToString(const Config::AltitudeAlarm en);

template<>
const char *ToString(const Config::FontSize en);

template<>
const char *ToString(const Config::AltitudeAlarm::AlarmAmplitude en);

template<>
const char *ToString(const Config::AltitudeAlarm::AlarmType en);

template<>
const char *ToString(const UiAltimeter::AltimeterUiMode en);

template<>
const char *ToString(const Config::DataSaveMode en);

template<>
const char *ToString(const Config::AltitudeDisplayRoundMode en);

template<>
const char *ToString(const Config::AltitudeSensorOversampleRate en);

template<typename T>
T NextEnum(const T en)
{
    return (T)0;
}

template<>
Config::AltitudeDisplayUnitMode NextEnum(const Config::AltitudeDisplayUnitMode en);

template<>
Config::SpeedDisplayUnitMode NextEnum(const Config::SpeedDisplayUnitMode en);

template<>
Config::TemperatureMode NextEnum(const Config::TemperatureMode en);

template<>
axlib::DisplayBuffer::Rotation NextEnum(const axlib::DisplayBuffer::Rotation en);

template<>
Config::AltitudeAlarm::AlarmAmplitude NextEnum(const Config::AltitudeAlarm::AlarmAmplitude en);

template<>
Config::AltitudeAlarm::AlarmType NextEnum(const Config::AltitudeAlarm::AlarmType en);

template<>
Config::FontSize NextEnum(const Config::FontSize en);

template<>
UiAltimeter::AltimeterUiMode NextEnum(const UiAltimeter::AltimeterUiMode en);

template<>
Config::DataSaveMode NextEnum(const Config::DataSaveMode en);

template<>
Config::AltitudeDisplayRoundMode NextEnum(const Config::AltitudeDisplayRoundMode en);

template<>
Config::AltitudeSensorOversampleRate NextEnum(const Config::AltitudeSensorOversampleRate en);

uint8_t GetVolume(const Config::AltitudeAlarm::AlarmAmplitude amp);

#endif //ifndef CONFIG_HH
