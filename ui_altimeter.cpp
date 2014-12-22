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
    //buffer->RenderText(FontStyle_impact_huge, 3,0,GetAltitudeString());
    //buffer->RenderText(FontStyle_impact_huge, 3,0,1,2,GetAltitudeString());
    buffer->RenderText_AlignCenter(FontStyle_impact_huge, buffer->GetWidth()/2,0,1,2,GetAltitudeString());
    buffer->RenderText(FontStyle_impact,3,40+28,1,1,GetAltitudeChangeStringLong());
    buffer->RenderText(FontStyle_impact,3,55+28,1,1,GetAltitudeStringLong());
    buffer->RenderText(FontStyle_impact,3,70+28,1,1,GetTemperatureStringLong());
}

void UiAltimeter::KeyPress(const UiBase::KeyCode key, const bool down)
{

}

void UiAltimeter::Tick100ms()
{

}

char *UiAltimeter::GetAltitudeString()
{
    int16_t alt = sensors_->GetAltitudeMeters();
    // Clamp altitude to range that fits to the altitude string

    alt = std::max(MIN_UI_ALTITUDE,std::min(MAX_UI_ALTITUDE, alt));
    sprintf(altitude_string_,"%d",alt);
    return altitude_string_;
}

char *UiAltimeter::GetAltitudeStringLong()
{
    int16_t alt = sensors_->GetAltitudeMeters();
    alt = std::max(MIN_UI_ALTITUDE,std::min(MAX_UI_ALTITUDE, alt));
    sprintf(altitude_string_long_,"Alt: %d m",alt);
    return altitude_string_long_;
}

char *UiAltimeter::GetAltitudeChangeStringLong()
{
    int16_t change = sensors_->GetAltitudeChangeRateDecimetresPerS();
    change = std::max(MIN_UI_ALTITUDE_RATE,std::min(MAX_UI_ALTITUDE_RATE, change));

    if (change < 0) {
        sprintf(altitude_change_rate_string_long_, "Descent: %d.%d m/s", change/10, std::abs(change%10));
    } else {
        sprintf(altitude_change_rate_string_long_, "Climb: %d.%d m/s", change/10, change%10);
    }
    return altitude_change_rate_string_long_;
}

char *UiAltimeter::GetTemperatureStringLong()
{
    int16_t temp = sensors_->GetTemperatureC();
    temp = std::max(MIN_UI_TEMPERATURE,std::min(MAX_UI_TEMPERATURE, temp));
    sprintf(temperature_string_long_,"Temp: %d C",temp);
    return temperature_string_long_;
}
