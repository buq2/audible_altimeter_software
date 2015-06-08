#ifndef UI_ALTIMETER_HH
#define UI_ALTIMETER_HH

#include "ui_base.hh"
#include "sensors.hh"
#include "axlib/displays/display_buffer.hh"
#include "altitude_manager.hh"

class UiAltimeter
        :
        public UiBase
{
 public:
    UiAltimeter(Sensors *sensors, AltitudeManager *alt_manager);
    void Render(axlib::DisplayBuffer *buffer);
    void KeyPress(const UiBase::KeyCode key, const bool down);
    void Tick100ms();

    typedef enum AltimeterUiMode_t
    {
        ALTIMETER_UI_MODE_FREE_FALL = 0,
        ALTIMETER_UI_MODE_COMPLEX = 1,
        ALTIMETER_UI_MODE_NUM_ENUMS = 2
    } AltimeterUiMode;

    void SetUiMode(const AltimeterUiMode mode);

    static int16_t MAX_UI_ALTITUDE;
    static int16_t MIN_UI_ALTITUDE;
    static int16_t MIN_UI_ALTITUDE_RATE;
    static int16_t MAX_UI_ALTITUDE_RATE;
    static int16_t MAX_UI_TEMPERATURE;
    static int16_t MIN_UI_TEMPERATURE;
 private:
    void RenderComplex(axlib::DisplayBuffer *buffer);
    void RenderSimpleFreeFall(axlib::DisplayBuffer *buffer);

    void RenderAltitude(axlib::DisplayBuffer *buffer, uint8_t *row);
    void RenderAltitudeLong(axlib::DisplayBuffer *buffer, uint8_t *row);
    void RenderAltitudeChangeLong(axlib::DisplayBuffer *buffer, uint8_t *row);
    void RenderTemperatureLong(axlib::DisplayBuffer *buffer, uint8_t *row);

    void RenderMisc(axlib::DisplayBuffer *buffer, uint8_t *row);
    void RenderUpdateRate(axlib::DisplayBuffer *buffer, uint8_t *row);
    void RenderAltitudeMode(axlib::DisplayBuffer *buffer, uint8_t *row);
    void RenderMemoryUsage(axlib::DisplayBuffer *buffer, uint8_t *row);
 private:
    Sensors *sensors_;
    AltimeterUiMode mode_;
    AltitudeManager *alt_manager_;
};

#endif //ifndef UI_ALTIMETER_HH
