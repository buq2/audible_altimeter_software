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
      sensors_(sensors)
{

}

void UiAltimeter::Render(DisplayBuffer *buffer)
{
    buffer->Clear();
    uint8_t y = 0;
    RenderAltitude(buffer, &y);
    RenderAltitudeChangeLong(buffer, &y);
    RenderTemperatureLong(buffer, &y);

    //buffer->RenderText(FontStyle_impact_huge, 3,0,GetAltitudeString());
    //buffer->RenderText(FontStyle_impact_huge, 3,0,1,2,GetAltitudeString());
    //buffer->RenderText_AlignCenter(FontStyle_impact_huge, buffer->GetWidth()/2,0,1,2,GetAltitudeString());
    //buffer->RenderText(FontStyle_impact,3,40+28,1,1,GetAltitudeChangeStringLong());
    //buffer->RenderText(FontStyle_impact,3,55+28,1,1,GetAltitudeStringLong());
    //buffer->RenderText(FontStyle_impact,3,70+28,1,1,GetTemperatureStringLong());
}

void UiAltimeter::KeyPress(const UiBase::KeyCode key, const bool down)
{

}

void UiAltimeter::Tick100ms()
{

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
    char str[13]; //"Alt: -9999 m" + null
    int16_t alt = sensors_->GetAltitudeMeters();
    alt = std::max(MIN_UI_ALTITUDE,std::min(MAX_UI_ALTITUDE, alt));
    sprintf(str,"Alt: %d m",alt);

    const uint8_t xpos = 3;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 1;
    buffer->RenderText(FontStyle_impact, xpos,
                       *row, scale_x, scale_y, str);
    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_impact, scale_y, str);
}

void UiAltimeter::RenderAltitudeChangeLong(DisplayBuffer *buffer, uint8_t *row)
{
    char str[20]; //"Descent: 999.9 ft/s" + null

    int16_t change = sensors_->GetAltitudeChangeRateDecimetresPerS();
    change = std::max(MIN_UI_ALTITUDE_RATE,std::min(MAX_UI_ALTITUDE_RATE, change));

    if (change < 0) {
        sprintf(str, "Descent: %d.%d m/s", change/10, std::abs(change%10));
    } else {
        sprintf(str, "Climb: %d.%d m/s", change/10, change%10);
    }

    const uint8_t xpos = 3;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 1;
    buffer->RenderText(FontStyle_impact, xpos,
                       *row, scale_x, scale_y, str);
    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_impact, scale_y, str);
}

void UiAltimeter::RenderTemperatureLong(DisplayBuffer *buffer, uint8_t *row)
{
    char str[14]; //"Temp: -999 °F"

    int16_t temp = sensors_->GetTemperatureC();
    temp = std::max(MIN_UI_TEMPERATURE,std::min(MAX_UI_TEMPERATURE, temp));
    sprintf(str,"Temp: %d C",temp);

    const uint8_t xpos = 3;
    const uint8_t scale_x = 1;
    const uint8_t scale_y = 1;
    buffer->RenderText(FontStyle_impact, xpos,
                       *row, scale_x, scale_y, str);
    *row += DisplayBuffer::CalculateTextHeightPixels(FontStyle_impact, scale_y, str);
}
