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

    /// Create request of data update for the sensors
    /// Required to be called before data is read from the sensors as
    /// it takes some time to convert the analog sensor value to digital
    void RequestDataUpdate();

    /// Should be called when sensor data should be read
    /// \param[in] time_since_update How many seconds it is from last update
    void DataUpdate(const float time_since_update_s);

    /// Zero current altitude. Basically current raw height is read from the
    /// altimeter and the set as the ground level
    void ZeroAltitude();

    /// Start saving jump data
    void StartSavingData();

    /// Stop saving jump data
    void StopSavingData();

    /// Save data to flash if data saving is enabled
    /// \param[in] time_since_update How many seconds a go the data was last time updated
    void SaveDataIfEnabled(const float time_since_update);

    /// Start new jump
    void StartNewJump();

    /// Set misc informatio pointer
    void SetMiscInformation(MiscInformation *misc);

    /// Fill buffer with jump data
    /// \param[in] time_since_update How many seconds a go the data was last time updated
    /// \parma[in,out] buffer Pointer to buffer which should be filled
    /// \param[out] num_bytes Number of bytes written to the buffer
    void FillJumpDataBuffer(const float time_since_update, uint8_t **buffer, uint8_t *num_bytes);

    /// Quick erase the flash memory
    void QuickErase();

    /// Quick erase jumps memory
    void QuickEraseJumps();

    /// Fully erase the flash memory
    void FullErase();

    /// Set if fake data should be used instead of reading the actual sensor data
    /// \param[in] use_fake If true, sensor data will not be read. Altitude data
    ///     will be created from parameters
    void SetUseFakeData(const bool use_fake);

    /// Set current fake altitude
    /// \param[in] altitude_m Altitude in meters
    void SetFakeAltitude(const float altitude_m);

    /// Set fake altitude rate of change
    /// \param[in] altitude_change_m_per_s How much altitude changes in one second
    void SetFakeAltitudeChange(const float altitude_change_m_per_s);

    /// Types of supported demos
    typedef enum DemoType_t {
        DEMO_CLIMB,
        DEMO_FREEFALL,
        DEMO_CANOPY,
        DEMO_GROUND,
        DEMO_OFF
    } DemoType;

    /// Run one of the demos
    /// \param[in] demo Demo to be run
    /// \note After the call fake data will be used
    void RunDemo(const DemoType demo);
 private:
    /// Update demo values
    void UpdateDemo();
 private:
    // If true, data will be saved
    bool save_data_;

    // Pointer to data storage
    Sensors *sensors_;

    // Memory controller
    MemoryController *mem_control_;

    // Clock
    axlib::ClockMcp7940M *clock_;

    // Altimeters
    axlib::AltimeterMPl3114A2 alt1_;
    //axlib::AltimeterMS5805_02BA01 alt2_;

    // Misc information which should be updated
    MiscInformation *misc_;

    // Current memory address
    uint32_t flash_current_memory_address_;

    // Buffer of jump data
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
