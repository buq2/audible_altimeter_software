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
 private:
    Sensors *sensors_;
    MemoryController *mem_control_;
    axlib::ClockMcp7940M *clock_;
    axlib::AltimeterMPl3114A2 alt1_;
    axlib::AltimeterMS5805_02BA01 alt2_;
}; //class SensorController

#endif //ifndef AUDIBLEA_SENSOR_CONTOLLER_HH
