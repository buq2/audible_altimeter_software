#ifndef AUDIBLEA_SENSOR_CONTOLLER_HH
#define AUDIBLEA_SENSOR_CONTOLLER_HH

#include "axlib/sensors/altimeter_mpl3115a2.hh"
#include "axlib/sensors/altimeter_ms5805_02ba01.hh"
#include "axlib/memory/flash_s25fl216k.hh"
#include "axlib/sensors/clock_mcp7940m.hh"
#include "sensors.hh"
#include "memory.hh"

class SensorController
{
 public:
    SensorController(Sensors *sensors,
                     MemoryController *mem_control,
                     axlib::ClockMcp7940M *clock);
    void Setup();
    void RequestDataUpdate();
    void DataUpdate(const float time_since_update);
    void ZeroAltitude();
    void StartSavingData();
    void StopSavingData();
    void SaveDataIfEnabled(const float time_since_update);
    void StartNewJump();
    void SetMiscInformation(MiscInformation *misc);
    void FillJumpDataBuffer(const float time_since_update, uint8_t **buffer, uint8_t *num_bytes);
    void QuickErase();
    void FullErase();

    void SetUseFakeData(const bool use_fake);
    void SetFakeAltitude(const float altitude_m);
    void SetFakeAltitudeChange(const float altitude_change_m_per_s);

    typedef enum DemoType_t {
        DEMO_CLIMB,
        DEMO_FREEFALL,
        DEMO_CANOPY,
        DEMO_GROUND,
        DEMO_OFF
    } DemoType;
    void RunDemo(const DemoType demo);
 private:
    void UpdateDemo();
 private:
    bool save_data_;
    Sensors *sensors_;
    MemoryController *mem_control_;
    axlib::ClockMcp7940M *clock_;
    axlib::AltimeterMPl3114A2 alt1_;
    axlib::AltimeterMS5805_02BA01 alt2_;
    MiscInformation *misc_;

    uint32_t flash_current_memory_address_;
    uint8_t jump_data_buffer_[64];

    // If true, data is not read from the sensors, but from external
    // input (serial console?)
    bool use_fake_data_;

    // Fake altitude which is modified by altitude change speed
    float fake_altitude_;

    // Fake altitude change
    float fake_altitude_change_;

    DemoType demo_;
}; //class SensorController

#endif //ifndef AUDIBLEA_SENSOR_CONTOLLER_HH
