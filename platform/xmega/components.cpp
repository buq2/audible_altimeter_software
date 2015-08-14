#include "components.hh"

Components *global_components_;

Components *GetComponents()
{
    return global_components_;
}

void SetComponents(Components *comp)
{
    global_components_ = comp;
}

Components::Components()
    :
      buttons_(PORT_D,PIN_2, //up
               PORT_D,PIN_1, //center
               PORT_D,PIN_0), //down
      digipot_(PORT_C),
      display_buffer_(128,128),
      display_(128,128,
               PORT_C,  //spi
               PORT_A, PIN_3, //cs
               PORT_NOT_USED, 0, //excomin
               PORT_A, PIN_2 //display on
               ),
      alt_manager_(&sensors_,&config_),
      ui_main_(&config_, &sensors_, &misc_info_, &alt_manager_),
      clock_(PORT_C //i2c port
             ),
      flash_(PORT_C, //spi port
             PORT_A, //cs port
             PIN_4 //cs pin
             ),
      mem_control_(&flash_),
      sensor_ctrl_(&sensors_, &mem_control_, &clock_),
      buzzer_(PORT_D, //buzzer port
              PIN_4, //buzzer pin
              &digipot_)
{
}

UiMain *Components::GetUiMain()
{
    return &ui_main_;
}

DisplayBuffer *Components::GetDisplayBuffer()
{
    return &display_buffer_;
}

Buttons *Components::GetButtons()
{
    return &buttons_;
}

DigipotMcp4017T *Components::GetDigipot()
{
    return &digipot_;
}

DisplaySharp *Components::GetDisplay()
{
    return &display_;
}

SensorController *Components::GetSensorController()
{
    return &sensor_ctrl_;
}

MemoryController *Components::GetMemoryController()
{
    return &mem_control_;
}

Sensors *Components::GetSensors()
{
    return &sensors_;
}

ClockMcp7940M *Components::GetClock()
{
    return &clock_;
}

MiscInformation *Components::GetMiscInformation()
{
    return &misc_info_;
}

Config *Components::GetConfig()
{
    return &config_;
}

FlashS25Fl216K *Components::GetFlash()
{
    return &flash_;
}

Buzzer *Components::GetBuzzer()
{
    return &buzzer_;
}

AltitudeManager *Components::GetAltitudeManager()
{
    return &alt_manager_;
}
