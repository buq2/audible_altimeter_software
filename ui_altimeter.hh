#ifndef UI_ALTIMETER_HH
#define UI_ALTIMETER_HH

#include "ui_base.hh"
#include "sensors.hh"

class UiAltimeter
        :
        public UiBase
{
 public:
    UiAltimeter(Sensors *sensors);
    void Render(DisplayBuffer *buffer);
    void KeyPress(const UiBase::KeyCode key, const bool down);
    void Tick100ms();

    static int16_t MAX_UI_ALTITUDE;
    static int16_t MIN_UI_ALTITUDE;
    static int16_t MIN_UI_ALTITUDE_RATE;
    static int16_t MAX_UI_ALTITUDE_RATE;
    static int16_t MAX_UI_TEMPERATURE;
    static int16_t MIN_UI_TEMPERATURE;
 private:
    char *GetAltitudeString();
    char *GetAltitudeStringLong();
    char *GetAltitudeChangeStringLong();
    char *GetTemperatureStringLong();
 private:
    Sensors *sensors_;
    char altitude_string_[6]; //"-9999" + null
    char altitude_string_long_[13]; //"Alt: -9999 m" + null
    char altitude_change_rate_string_long_[20]; //"Descent: 999.9 ft/s" + null
    char temperature_string_long_[14]; //"Temp: -999 °F"
};

#endif //ifndef UI_ALTIMETER_HH
