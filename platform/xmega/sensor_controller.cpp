#include "sensor_controller.hh"
#include <string.h>
#include <stdlib.h>

using namespace axlib;

#define STOP_IF_ERROR(x) {if(x){while(1){}}}

SensorController::SensorController(Sensors *sensors, MemoryController *mem_control, axlib::ClockMcp7940M *clock)
    :
      sensors_(sensors),
      mem_control_(mem_control),
      clock_(clock),
      alt1_(PORT_C),
      alt2_(PORT_C),
      misc_(0),
      flash_current_memory_address_(0),
      use_fake_data_(false),
      fake_altitude_(0),
      fake_altitude_change_(0)
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

void SensorController::UpdateDemo()
{
    switch(demo_) {
    case DEMO_CLIMB:
        fake_altitude_change_ = 5;
        if (fake_altitude_ >= 1600) {
            demo_ = DEMO_FREEFALL;
        }
        break;
    case DEMO_FREEFALL:
        fake_altitude_change_ = -55;
        if (fake_altitude_ <= 900 && fake_altitude_ > 800) {
            fake_altitude_change_ = -33;
        } else if (fake_altitude_ <= 800) {
            demo_ = DEMO_CANOPY;
        }
        break;
    case DEMO_CANOPY:
        fake_altitude_change_ = -8;
        if (fake_altitude_ <= 5) {
            demo_ = DEMO_GROUND;
        }
        break;
    case DEMO_GROUND:
        fake_altitude_ = (rand()%10)/5.0f;
        fake_altitude_change_ = 0;
        break;
    default:
        break;
    }
}

void SensorController::DataUpdate(const float time_since_update)
{
    if (!use_fake_data_) {
        // Update altitude
        float altitude_m = 0;
        if (0 == alt1_.GetAltitudeMeters(&altitude_m)) {
            sensors_->SetAltitudeMeters(altitude_m, time_since_update);
        }
    } else {
        // Use only fake data
        fake_altitude_ = fake_altitude_ + time_since_update*fake_altitude_change_;
        sensors_->SetAltitudeMeters(fake_altitude_, time_since_update);
        if (demo_ != DEMO_OFF) {
            UpdateDemo();
        }
    }
    float temperature_c = 0;
    if (0 == alt1_.GetTemperature(&temperature_c)) {
        sensors_->SetTemperatureC(temperature_c);
    }

    sensors_->SetUpdateRate(1.0f/time_since_update);

    SaveDataIfEnabled(time_since_update);
}

void SensorController::ZeroAltitude()
{
    alt1_.ZeroAltitude();
}

void SensorController::StartSavingData()
{
    if (save_data_) {
        // Already saving
        return;
    }

    StartNewJump();

    save_data_ = true;
}

void SensorController::StopSavingData()
{
    save_data_ = false;
}

void SensorController::SaveDataIfEnabled(const float time_since_update)
{
    if (!save_data_) {
        return;
    }

    uint8_t *ptr;
    uint8_t num_bytes;
    FillJumpDataBuffer(time_since_update, &ptr, &num_bytes);
    mem_control_->WriteJumpData(ptr, num_bytes, &flash_current_memory_address_);

    // Update memory usage
    if (0 != misc_) {
        misc_->current_memory_usage = flash_current_memory_address_;
    }
}

void SensorController::StartNewJump()
{
    flash_current_memory_address_ = mem_control_->InitJumpSector_ForNewJump();
}

void SensorController::SetMiscInformation(MiscInformation *misc)
{
    misc_ = misc;
}

void SensorController::FillJumpDataBuffer(const float time_since_update,
                                          uint8_t **buffer, uint8_t *num_bytes)
{
    *buffer = jump_data_buffer_;
    *num_bytes = 0;
    uint8_t *ptr = *buffer;


#define AUDIBLEA_COPY_DATA_MACRO(x) \
    uint8_t b = sizeof(x);\
    memcpy(ptr, &x, b);\
    ptr += b; \
    *num_bytes += b;

#define AUDIBLEA_ADD_STRUCT_TYPE(x) \
    uint8_t data = x;\
    memcpy(ptr, &data, 1);\
    ptr += 1; \
    *num_bytes += 1;

    {
        JumpData_BeginSensorData begin;
        begin.time_since_last_sensor_data_seconds = time_since_update;

        AUDIBLEA_ADD_STRUCT_TYPE(JUMPDATA_STRUCT_ENUM_BEGIN_SENSOR_DATA);
        AUDIBLEA_COPY_DATA_MACRO(begin);
    }

    {
        JumpData_Altitude alt1;
        alt1.altitude_sensor_id = 1;
        alt1_.GetAltitudeMeters(&(alt1.altitude_meters));

        AUDIBLEA_ADD_STRUCT_TYPE(JUMPDATA_STRUCT_ENUM_ALTITUDE);
        AUDIBLEA_COPY_DATA_MACRO(alt1);
    }

    {
        JumpData_AltitudeRate rate;
        rate.altitude_rate_m_per_s = sensors_->GetAltitudeChangeRateMetresPerS();

        AUDIBLEA_ADD_STRUCT_TYPE(JUMPDATA_STRUCT_ENUM_ALTITUDE_RATE);
        AUDIBLEA_COPY_DATA_MACRO(rate);
    }
#undef AUDIBLEA_COPY_DATA_MACRO
}

void SensorController::QuickErase()
{
    StopSavingData();
    mem_control_->QuickErase();
}

void SensorController::FullErase()
{
    StopSavingData();
    mem_control_->FullErase();
}

void SensorController::SetUseFakeData(const bool use_fake)
{
    use_fake_data_ = use_fake;
}

void SensorController::SetFakeAltitude(const float altitude_m)
{
    fake_altitude_ = altitude_m;
}

void SensorController::SetFakeAltitudeChange(const float altitude_change_m_per_s)
{
    fake_altitude_change_ = altitude_change_m_per_s;
}

void SensorController::RunDemo(const SensorController::DemoType demo)
{
    switch(demo) {
    case DEMO_CLIMB:
        SetFakeAltitude(0);
        SetUseFakeData(true);
        break;
    case DEMO_FREEFALL:
        SetFakeAltitude(4000);
        SetUseFakeData(true);
        break;
    case DEMO_CANOPY:
        SetFakeAltitude(900);
        SetUseFakeData(true);
        break;
    case DEMO_GROUND:
        SetFakeAltitude(0);
        SetUseFakeData(true);
        break;
    case DEMO_OFF:
        SetUseFakeData(false);
    default:
        break;
    }
    demo_ = demo;
}
