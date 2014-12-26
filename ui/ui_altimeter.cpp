#include "ui_altimeter.hh"
#include "fonts/fontlibrary.h"
#include <boost/lexical_cast.hpp>
#include <iostream>

int16_t UiAltimeter::MAX_UI_ALTITUDE = 32767;
int16_t UiAltimeter::MIN_UI_ALTITUDE = -9999;

int16_t UiAltimeter::MAX_UI_ALTITUDE_RATE = 9999;
int16_t UiAltimeter::MIN_UI_ALTITUDE_RATE = -9999;

int16_t UiAltimeter::MAX_UI_TEMPERATURE = 999;
int16_t UiAltimeter::MIN_UI_TEMPERATURE = -999;

UiAltimeter::UiAltimeter(Sensors *sensors)
    :
      sensors_(sensors),
      mode_(ALTIMETER_UI_MODE_COMPLEX)
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

void UiAltimeter::RenderComplex(DisplayBuffer *buffer)
{
    uint8_t y = 0;
    RenderAltitude(buffer, &y);
    RenderAltitudeLong(buffer, &y);
    RenderAltitudeChangeLong(buffer, &y);
    RenderTemperatureLong(buffer, &y);
}

void UiAltimeter::RenderSimpleFreeFall(DisplayBuffer *buffer)
{
    int16_t alt = sensors_->GetAltitudeMeters();
    char str[6];
    sprintf(str,"%d.%d",alt/1000, std::abs((alt/100)%10) );
    const uint8_t scale_x = 2;
    const uint8_t scale_y = 2;
    const uint8_t xpos = buffer->GetWidth()/2;
    const uint8_t ypos = buffer->GetHeight()/2 - DisplayBuffer::CalculateTextHeightPixels(FontStyle_impact_huge, scale_y, str)/2;
    buffer->RenderText_AlignCenter(FontStyle_impact_huge, xpos,
                                   ypos, scale_x, scale_y, str);
}

void UiAltimeter::RenderAltitude(DisplayBuffer *buffer, uint8_t *row)
{
    char str[6]; //"-9999" + null
    int16_t alt = sensors_->GetAltitudeMeters();
    // Clamp altitude to range that fits to the altitude string

    alt = std::max(MIN_UI_ALTITUDE,std::min(MAX_UI_ALTITUDE, alt));
    sprintf(str,"%d",alt);

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
    alt = std::max(MIN_UI_ALTITUDE,std::min(MAX_UI_ALTITUDE, alt));
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

    int16_t change = sensors_->GetAltitudeChangeRateDecimetresPerS();
    change = std::max(MIN_UI_ALTITUDE_RATE,std::min(MAX_UI_ALTITUDE_RATE, change));

    const char *str_desc = "Descent:";
    const char *str_climb = "Climb:";
    const char *str_front = str_desc;
    if (change < 0) {
        str_front = str_desc;
        sprintf(str, "%d.%d m/s", change/10, std::abs(change%10));
    } else {
        str_front = str_climb;
        sprintf(str, "%d.%d m/s", change/10, change%10);
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
    temp = std::max(MIN_UI_TEMPERATURE,std::min(MAX_UI_TEMPERATURE, temp));
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
