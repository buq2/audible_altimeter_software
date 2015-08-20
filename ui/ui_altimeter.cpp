#include "ui_altimeter.hh"
#include "axlib/displays/fonts/fontlibrary.h"
#include <string.h>
#include <stdio.h>
#include "axlib/core/core.hh"
#include "config.hh"
#include <math.h>

int16_t UiAltimeter::MAX_UI_ALTITUDE = 32767;
int16_t UiAltimeter::MIN_UI_ALTITUDE = -9999;

int16_t UiAltimeter::MAX_UI_ALTITUDE_RATE = 9999;
int16_t UiAltimeter::MIN_UI_ALTITUDE_RATE = -9999;

int16_t UiAltimeter::MAX_UI_TEMPERATURE = 999;
int16_t UiAltimeter::MIN_UI_TEMPERATURE = -999;

using namespace axlib;

UiAltimeter::UiAltimeter(Sensors *sensors, AltitudeManager *alt_manager, Config *config)
    :
      sensors_(sensors),
      mode_(ALTIMETER_UI_MODE_COMPLEX),
      alt_manager_(alt_manager),
      config_(config)
{

}

void UiAltimeter::Render(DisplayBuffer *buffer)
{
    buffer->Clear();
    switch(mode_) {
    case ALTIMETER_UI_MODE_COMPLEX:
        RenderComplex(buffer);
        break;
    case ALTIMETER_UI_MODE_FREE_FALL:
    default:
        RenderSimpleFreeFall(buffer);
        break;
    }
}

void UiAltimeter::KeyPress(const UiBase::KeyCode /*key*/, const bool /*down*/)
{

}

void UiAltimeter::Tick100ms()
{

}

void UiAltimeter::SetUiMode(const UiAltimeter::AltimeterUiMode mode)
{
    mode_ = mode;
}

void UiAltimeter::RenderComplex(DisplayBuffer *buffer)
{
    uint8_t y = 0;
    RenderAltitude(buffer, &y);
    //RenderAltitudeLong(buffer, &y);
    RenderAltitudeChangeLong(buffer, &y);
    //RenderTemperatureLong(buffer, &y);
    RenderMisc(buffer, &y);
}

void UiAltimeter::RenderSimpleFreeFall(DisplayBuffer *buffer)
{
    int16_t alt = sensors_->GetAltitudeMeters();
    char str[6];
    sprintf(str,"%d.%d",alt/1000, ABS((alt/100)%10) );
    const uint8_t scale_x = 2;
    const uint8_t scale_y = 2;
    const uint8_t xpos = buffer->GetWidth()/2;
    const uint8_t ypos = buffer->GetHeight()/2 - DisplayBuffer::CalculateTextHeightPixels(FontStyle_impact_huge, scale_y, str)/2;
    buffer->RenderText_AlignCenter(FontStyle_impact_huge, xpos,
                                   ypos, scale_x, scale_y, str);
}

void UiAltimeter::RenderAltitude(DisplayBuffer *buffer, uint8_t *row)
{
    char str[8]; //"-9999.0" + null
    float alt = sensors_->GetAltitudeMeters();
    // Clamp altitude to range that fits to the altitude string

    alt = MAX(MIN_UI_ALTITUDE,MIN(MAX_UI_ALTITUDE, alt));

    switch (config_->display_round_mode) {
    case Config::AltitudeDisplayRoundMode1:
        sprintf(str,"%.0f",ceil(alt-0.5f));
        break;
    case Config::AltitudeDisplayRoundMode5:
        sprintf(str,"%.0f",ceil(alt/5.0f-0.5f)*5.0f);
        break;
    case Config::AltitudeDisplayRoundMode10:
        sprintf(str,"%.0f",ceil(alt/10.0f-0.5f)*10.0f);
        break;
    default:
    case Config::AltitudeDisplayRoundModeNone:
        sprintf(str,"%.1f",alt);
        break;
    }


    const uint8_t xpos = buffer->GetWidth()/2;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 2;
    buffer->RenderText_AlignCenter(FontStyle_impact_huge, xpos,
                                   *row, scale_x, scale_y, str);
    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_impact_huge, scale_y, str);
}

void UiAltimeter::RenderAltitudeLong(DisplayBuffer *buffer, uint8_t *row)
{
    char str[8]; //"-9999 m" + null
    int16_t alt = sensors_->GetAltitudeMeters();
    alt = MAX(MIN_UI_ALTITUDE,MIN(MAX_UI_ALTITUDE, alt));
    sprintf(str,"%d m",alt);

    uint8_t xpos = 3;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 1;
    buffer->RenderText(FontStyle_impact, xpos,
                       *row, scale_x, scale_y, "Alt:");
    xpos = buffer->GetWidth()-3;
    buffer->RenderText_AlignRight(FontStyle_impact, xpos,
                       *row, scale_x, scale_y, str);
    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_impact, scale_y, str);
}

void UiAltimeter::RenderAltitudeChangeLong(DisplayBuffer *buffer, uint8_t *row)
{
    char str[11]; //"999.9 ft/s" + null

    float change = sensors_->GetAltitudeChangeRateMetresPerS();
    change = MAX(MIN_UI_ALTITUDE_RATE,MIN(MAX_UI_ALTITUDE_RATE, change));

    const char *str_desc = "Desc:";
    const char *str_climb = "Clmb:";
    const char *str_front = str_desc;

    sprintf(str, "%0.1f m/s", change);
    if (change < 0) {
        str_front = str_desc;
    } else {
        str_front = str_climb;
    }

    uint8_t xpos = 3;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 1;
    buffer->RenderText(FontStyle_impact, xpos,
                       *row, scale_x, scale_y, str_front);
    xpos = buffer->GetWidth()-3;
    buffer->RenderText_AlignRight(FontStyle_impact, xpos,
                       *row, scale_x, scale_y, str);
    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_impact, scale_y, str);
}

void UiAltimeter::RenderTemperatureLong(DisplayBuffer *buffer, uint8_t *row)
{
    char str[8]; //"-999 °F"

    int16_t temp = sensors_->GetTemperatureC();
    temp = MAX(MIN_UI_TEMPERATURE,MIN(MAX_UI_TEMPERATURE, temp));
    sprintf(str,"%d C",temp);

    uint8_t xpos = 3;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 1;
    buffer->RenderText(FontStyle_impact, xpos,
                       *row, scale_x, scale_y, "Temp:");
    xpos = buffer->GetWidth()-3;
    buffer->RenderText_AlignRight(FontStyle_impact, xpos,
                       *row, scale_x, scale_y, str);
    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_impact, scale_y, str);
}

void UiAltimeter::RenderMisc(DisplayBuffer *buffer, uint8_t *row)
{
    // Render at the same row
    uint8_t row_cur = *row;
    //RenderUpdateRate(buffer,row);
    RenderAltitudeMode(buffer,row);
    *row = row_cur;
    RenderMemoryUsage(buffer, row);
    RenderBatteryUsage(buffer,row);
}

void UiAltimeter::RenderUpdateRate(DisplayBuffer *buffer, uint8_t *row)
{
    char str[9]; //"10.2 fps"

    float fps = sensors_->GetUpdateRate();
    fps = MAX(0,MIN(99, fps));
    sprintf(str,"%0.1f fps",fps);

    float xpos = buffer->GetWidth()-3;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 1;
    buffer->RenderText_AlignRight(FontStyle_vcr_tiny, xpos,
                       *row, scale_x, scale_y, str);

    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_vcr_tiny, scale_y, str);
}

void UiAltimeter::RenderAltitudeMode(DisplayBuffer *buffer, uint8_t *row)
{
    static const char canopy[] = "Canopy";
    static const char free[] = "Freefall";
    static const char plane[] = "Plane";
    static const char ground[] = "Ground";
    const char *str;
    switch(alt_manager_->GetCurrentModeSimple()) {
    case AltitudeManager::AltitudeModeSimpleCanopy:
        str = canopy;
        break;
    case AltitudeManager::AltitudeModeSimpleFreefall:
        str = free;
        break;
    case AltitudeManager::AltitudeModeSimplePlane:
        str = plane;
        break;
    case AltitudeManager::AltitudeModeSimpleGround:
    default:
        str = ground;
        break;
    }

    float xpos = buffer->GetWidth()-3;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 1;
    buffer->RenderText_AlignRight(FontStyle_vcr_tiny, xpos,
                       *row, scale_x, scale_y, str);

    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_vcr_tiny, scale_y, str);
}

void UiAltimeter::RenderMemoryUsage(DisplayBuffer *buffer, uint8_t *row)
{
    MiscInformation *misc = sensors_->GetMiscInformation();
    if (NULL == misc) {
        return;
    }

    char str[10]; //"4294967296"
    sprintf(str,"%ld", misc->current_memory_usage);

    float xpos = 3;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 1;
    buffer->RenderText(FontStyle_vcr_tiny, xpos,
                       *row, scale_x, scale_y, str);

    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_vcr_tiny, scale_y, str);
}

void UiAltimeter::RenderBatteryUsage(DisplayBuffer *buffer, uint8_t *row)
{
    MiscInformation *misc = sensors_->GetMiscInformation();
    if (NULL == misc) {
        return;
    }

    char str[17]; //"Bat: 3.33V/3.33V"
    sprintf(str,"Bat: %0.2fV/3.33V", misc->current_battery_voltage);

    float xpos = 3;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 1;
    buffer->RenderText(FontStyle_vcr_tiny, xpos,
                       *row, scale_x, scale_y, str);

    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_vcr_tiny, scale_y, str);
}
