#ifndef SENSORS_HH
#define SENSORS_HH

#include <stdint.h>

class MiscInformation
{
 public:
    MiscInformation();

    uint32_t current_memory_usage;
    float current_battery_voltage;
};


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
    float GetUpdateRate();
    void SetUpdateRate(float fps);
    MiscInformation *GetMiscInformation();
    void SetMiscInformation(MiscInformation *misc);
 private:
 private:
#define SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER 6
    float altitude_m_;
    float altitudes_m_[SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER];
    float altitudes_since_previous_update_s_[SENSORS_NUMBER_OF_ALTITUDES_TO_REMEMBER];
    uint8_t next_altitude_change_idx_;
    int16_t temperature_c_;
    char altitude_string_[20];
    float update_rate_;
    MiscInformation *misc_;
}; //class Sensors

#endif //ifndef SENSORS_HH
