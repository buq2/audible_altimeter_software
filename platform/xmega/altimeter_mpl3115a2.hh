#ifndef ALTIMETER_MPL3114A2
#define ALTIMETER_MPL3114A2

#include <stdint.h>

class AltimeterMPl3114A2
{
 public:
    AltimeterMPl3114A2();

    typedef enum {
        OversampleRate1,
        OversampleRate2,
        OversampleRate4,
        OversampleRate8,
        OversampleRate16,
        OversampleRate32,
        OversampleRate64,
        OversampleRate128
    } OversampleRate;

    void SetOversampleRate(const OversampleRate rate);

    /// \return 0 for no error
    uint8_t Setup();

    /// If not altimeter -> pressure sensor
    void SetMode(const bool altimeter);

    /// 0) Send request for altitude update
    uint8_t RequestDataUpdate();

    /// 1) Check if altitude can be fetched
    /// \return 0 if altitude is not ready
    uint8_t DataReady();

    /// 2) Get altitude
    uint8_t GetAltitudeMeters(float *altitude);

    /// 2) Get temperature
    uint8_t GetTemperature(float *temperature);

    /// 2) Get Pressure
    uint8_t GetPressurePascals(float *pressure);

    char *GetAltitudeMetersString();

    /// Zeros altitude (will not fail but can get stuck)
    void ZeroAltitudeLoop();
 private:
    /// Gets current altitude and sets it as a reference point
    uint8_t ZeroAltitude();

    /// Altitude without zeroing
    uint8_t GetRawAltitudeMeters(float *altitude);
 private:
    char altitude_string_[20];
    bool request_data_called_;
    float altitude_ground_;
    uint8_t configuration_data_;

    float previous_raw_altitude_;
    float previous_pressure_;
    float previous_temperature_;
}; //class AltimeterMPl3114A2

#endif //ifndef ALTIMETER_MPL3114A2
