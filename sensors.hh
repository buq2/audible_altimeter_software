#ifndef SENSORS_HH
#define SENSORS_HH

#include <stdint.h>

class Sensors
{
 public:
    Sensors();
    float GetAltitudeMeters();
    float GetAltitudeChangeRateMetresPerS();
    int16_t GetTemperatureC();
    void SetAltitudeMeters(float altitude_meters, float time_since_update_seconds);
    char *GetAltitudeMetersString();
    void SetTemperatureC(int16_t temp);
 private:
 private:
#define SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER 6
    float altitude_m_;
    float altitudes_m_[SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER];
    float altitudes_since_previous_update_s_[SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER];
    uint8_t next_altitude_change_idx_;
    int16_t temperature_c_;
    char altitude_string_[20];
}; //class Sensors

#endif //ifndef SENSORS_HH
