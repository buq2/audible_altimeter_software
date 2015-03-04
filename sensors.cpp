#include "sensors.hh"

Sensors::Sensors()
    :
      altitude_m_(9999),
      altitude_change_dm_per_s_(13),
      temperature_c_(21)
{

}

float Sensors::GetAltitudeMeters()
{
    return altitude_m_;
}

float Sensors::GetAltitudeChangeRateDecimetresPerS()
{
    return altitude_change_dm_per_s_;
}

int16_t Sensors::GetTemperatureC()
{
    return temperature_c_;
}

void Sensors::SetAltitudeMeters(float altitude_meters)
{
    altitude_m_ = altitude_meters;
}
