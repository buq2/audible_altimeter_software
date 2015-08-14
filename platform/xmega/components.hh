#ifndef AUDIBLEA_COMPONENTS_HH
#define AUDIBLEA_COMPONENTS_HH

#include "config.hh"
#include "axlib/displays/display_buffer.hh"
#include "sensors.hh"
#include "ui/ui_main.hh"
#include "buttons.hh"
#include "axlib/displays/display_sharp.hh"
#include "axlib/sensors/clock_mcp7940m.hh"
#include "axlib/memory/flash_s25fl216k.hh"
#include "memory.hh"
#include "sensor_controller.hh"
#include "buzzer.hh"
#include "axlib/sensors/digipot_mcp4017t.hh"
#include "altitude_manager.hh"
#include "power.hh"

class Components
{
 public:
    Components();

    UiMain *GetUiMain();
    DisplayBuffer *GetDisplayBuffer();
    Buttons *GetButtons();
    DigipotMcp4017T *GetDigipot();
    DisplaySharp *GetDisplay();
    SensorController *GetSensorController();
    MemoryController *GetMemoryController();
    Sensors *GetSensors();
    ClockMcp7940M *GetClock();
    MiscInformation *GetMiscInformation();
    Config *GetConfig();
    FlashS25Fl216K *GetFlash();
    Buzzer *GetBuzzer();
    AltitudeManager *GetAltitudeManager();
    Power *GetPower();
 private:
    UiMain ui_main_;
    DisplayBuffer display_buffer_;
    Buttons buttons_;
    DigipotMcp4017T digipot_;
    DisplaySharp display_;
    SensorController sensor_ctrl_;
    MemoryController mem_control_;
    Sensors sensors_;
    ClockMcp7940M clock_;
    MiscInformation misc_info_;
    Config config_;
    FlashS25Fl216K flash_;
    Buzzer buzzer_;
    AltitudeManager alt_manager_;
    Power power_;
}; //class Components

Components *GetComponents();
void SetComponents(Components *comp);

#endif //ifndef AUDIBLEA_COMPONENTS_HH
