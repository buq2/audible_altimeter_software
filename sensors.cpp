#include "sensors.hh"

Sensors::Sensors()
    :
      altitude_m_(9999),
      altitude_change_dm_per_s_(13),
      temperature_c_(21)
{

}

int16_t Sensors::GetAltitudeMeters()
{
    // Debug code
    altitude_m_ -= 20;
    return altitude_m_;
}

int16_t Sensors::GetAltitudeChangeRateDecimetresPerS()
{
    return altitude_change_dm_per_s_;
}

int16_t Sensors::GetTemperatureC()
{
    return temperature_c_;
}
