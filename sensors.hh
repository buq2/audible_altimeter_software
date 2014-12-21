#ifndef SENSORS_HH
#define SENSORS_HH

#include <stdint.h>

class Sensors
{
 public:
    Sensors();
    int16_t GetAltitudeMeters();
    int16_t GetAltitudeChangeRateDecimetresPerS();
    int16_t GetTemperatureC();
 private:
 private:
    int16_t altitude_m_;
    int16_t altitude_change_dm_per_s_;
    int16_t temperature_c_;
}; //class Sensors

#endif //ifndef SENSORS_HH
