#include "sensor_controller.hh"

using namespace axlib;

#define STOP_IF_ERROR(x) {if(x){while(1){}}}

SensorController::SensorController(Sensors *sensors, MemoryController *mem_control, axlib::ClockMcp7940M *clock)
    :
      sensors_(sensors),
      mem_control_(mem_control),
      clock_(clock),
      alt1_(PORT_C),
      alt2_(PORT_C)
{

}

void SensorController::Setup()
{
    const bool as_altimeter = true;
    alt1_.SetMode(as_altimeter);
    alt1_.SetOversampleRate(AltimeterMPl3114A2::OversampleRate128);
    STOP_IF_ERROR(alt1_.Setup());
    STOP_IF_ERROR(alt2_.Setup());
}

void SensorController::RequestDataUpdate()
{
    alt1_.RequestDataUpdate();
}

void SensorController::DataUpdate(const float time_since_update)
{
    // Update altitude
    float altitude_m = 0;
    if (0 == alt1_.GetAltitudeMeters(&altitude_m)) {
        sensors_->SetAltitudeMeters(altitude_m, time_since_update);
    }
    float temperature_c = 0;
    if (0 == alt1_.GetTemperature(&temperature_c)) {
        sensors_->SetTemperatureC(temperature_c);
    }

    sensors_->SetUpdateRate(1.0f/time_since_update);
}

void SensorController::ZeroAltitude()
{
    alt1_.ZeroAltitude();
}
