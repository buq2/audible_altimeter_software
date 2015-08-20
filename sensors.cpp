#include "sensors.hh"
#include "stdio.h"

Sensors::Sensors()
    :
      altitude_m_(1000),
      temperature_c_(21),
      update_rate_(0),
      misc_(0)
{
    for (uint8_t i = 0; i < next_altitude_change_idx_; ++i) {
        altitudes_m_[i] = 0;
        altitudes_since_previous_update_s_[i] = 0;
    }
}

float Sensors::GetAltitudeMeters()
{
    return altitude_m_;
}

float Sensors::GetAltitudeChangeRateMetresPerS()
{
    float sum_time = 0;
    for (uint8_t i = 0; i < SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER; ++i) {
        sum_time += altitudes_since_previous_update_s_[i];
    }
    float sum_first = 0;
    float sum_second = 0;
    for (uint8_t i = 0; i < SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER/2; ++i) {
        sum_first += altitudes_m_[(next_altitude_change_idx_+i)%SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER];
        sum_second += altitudes_m_[(next_altitude_change_idx_+SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER/2+i)%SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER];
    }

    // Average change at this time
    const float half = SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER/2;
    return (sum_second/half-sum_first/half)/(sum_time/2);
}

int16_t Sensors::GetTemperatureC()
{
    return temperature_c_;
}

void Sensors::SetAltitudeMeters(float altitude_meters, float time_since_update_seconds)
{
    static float update_seconds = 0;
    update_seconds += time_since_update_seconds;
    // As sensor does not always update...
    if (altitude_meters == altitude_m_) {
        return;
    }
    altitudes_since_previous_update_s_[next_altitude_change_idx_] = update_seconds;
    altitudes_m_[next_altitude_change_idx_] = altitude_meters;
    ++next_altitude_change_idx_;
    next_altitude_change_idx_ %= SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER;
    altitude_m_ = altitude_meters;
    update_seconds = 0;
}

char *Sensors::GetAltitudeMetersString()
{
    float alt = GetAltitudeMeters();

    sprintf(altitude_string_,"%f\n\r",alt);
    return altitude_string_;
}

void Sensors::SetTemperatureC(int16_t temp)
{
    temperature_c_ = temp;
}

float Sensors::GetUpdateRate()
{
    return update_rate_;
}

void Sensors::SetUpdateRate(float fps)
{
    update_rate_ = fps;
}

MiscInformation *Sensors::GetMiscInformation()
{
    return misc_;
}

void Sensors::SetMiscInformation(MiscInformation *misc)
{
    misc_ = misc;
}

MiscInformation::MiscInformation()
    :
      current_memory_usage(0),
      current_battery_voltage(0)
{
}
