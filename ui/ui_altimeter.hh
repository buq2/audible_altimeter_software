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

    typedef enum AltimeterUiMode_t
    {
        ALTIMETER_UI_MODE_FREE_FALL,
        ALTIMETER_UI_MODE_COMPLEX
    } AltimeterUiMode;

    static int16_t MAX_UI_ALTITUDE;
    static int16_t MIN_UI_ALTITUDE;
    static int16_t MIN_UI_ALTITUDE_RATE;
    static int16_t MAX_UI_ALTITUDE_RATE;
    static int16_t MAX_UI_TEMPERATURE;
    static int16_t MIN_UI_TEMPERATURE;
 private:
    void RenderComplex(DisplayBuffer *buffer);
    void RenderSimpleFreeFall(DisplayBuffer *buffer);

    void RenderAltitude(DisplayBuffer *buffer, uint8_t *row);
    void RenderAltitudeLong(DisplayBuffer *buffer, uint8_t *row);
    void RenderAltitudeChangeLong(DisplayBuffer *buffer, uint8_t *row);
    void RenderTemperatureLong(DisplayBuffer *buffer, uint8_t *row);
 private:
    Sensors *sensors_;
    AltimeterUiMode mode_;
};

#endif //ifndef UI_ALTIMETER_HH
