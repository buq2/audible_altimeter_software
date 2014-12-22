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
    void RenderAltitude(DisplayBuffer *buffer, uint8_t *row);
    void RenderAltitudeLong(DisplayBuffer *buffer, uint8_t *row);
    void RenderAltitudeChangeLong(DisplayBuffer *buffer, uint8_t *row);
    void RenderTemperatureLong(DisplayBuffer *buffer, uint8_t *row);
 private:
    Sensors *sensors_;




};

#endif //ifndef UI_ALTIMETER_HH
