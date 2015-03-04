#ifndef SENSORS_HH
#define SENSORS_HH

#include <stdint.h>

class Sensors
{
 public:
    Sensors();
    float GetAltitudeMeters();
    float GetAltitudeChangeRateDecimetresPerS();
    int16_t GetTemperatureC();
    void SetAltitudeMeters(float altitude_meters);
 private:
 private:
    float altitude_m_;
    float altitude_change_dm_per_s_;
    int16_t temperature_c_;
}; //class Sensors

#endif //ifndef SENSORS_HH
