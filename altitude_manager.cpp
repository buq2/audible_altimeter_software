#include "altitude_manager.hh"
#include "axlib/core/core.hh"
#include "sensors.hh"
#include "config.hh"

AltitudeManager::AltitudeManager(Sensors *sensors, Config *config)
    :
      sensors_(sensors),
      config_(config)
{
    for (uint8_t i = 0; i < GetNumOfPreviousModes(); ++i) {
        previous_modes_[i] = AltitudeModeUnused;
    }
}

AltitudeManager::AltitudeMode AltitudeManager::GetCurrentMode()
{
    return current_mode_;
}

AltitudeManager::AltitudeModeSimple AltitudeManager::GetCurrentModeSimple()
{
    switch(current_mode_) {
    case AltitudeModePlane:
    case AltitudeModePlaneBeforeDrop:
        return AltitudeModeSimplePlane;
    case AltitudeModeFreefallBegin:
    case AltitudeModeFreefall:
    case AltitudeModeFreefallOpening:
        return AltitudeModeSimpleFreefall;
    case AltitudeModeCanopy:
        return AltitudeModeSimpleCanopy;
    case AltitudeModeGround:
    default:
        return AltitudeModeSimpleGround;
    }
}

void AltitudeManager::Tick200ms()
{
    float altitude_m = sensors_->GetAltitudeMeters();
    const float change_ms = sensors_->GetAltitudeChangeRateMetresPerS();

    AltitudeMode new_mode = AltitudeModeUnused;
    if (
            change_ms < -30 ||
            (MaybeInFreefall() && change_ms < -15))
    {
        new_mode = AltitudeModeFreefall;
    } else if (InPlane() && change_ms < -5) {
        new_mode = AltitudeModeFreefallBegin;
    } else if (InPlane() && axlib::abs(change_ms) < 1.5 && altitude_m > 600) {
        new_mode = AltitudeModePlaneBeforeDrop;
    } else if (change_ms < -1) {
        new_mode = AltitudeModeCanopy;
    } else if (altitude_m < 100 && axlib::abs(change_ms) <= 1) {
        new_mode = AltitudeModeGround;
    } else if (change_ms > 1) {
        new_mode = AltitudeModePlane;
    }

    previous_modes_[next_mode_index_] = new_mode;
    ++next_mode_index_;
    next_mode_index_ %= GetNumOfPreviousModes();

    CalculateCurrentMode();
}

bool AltitudeManager::InPlane()
{
    return current_mode_ == AltitudeModePlane || current_mode_ == AltitudeModePlaneBeforeDrop;
}

bool AltitudeManager::MaybeInFreefall()
{
    if (current_mode_ == AltitudeModeFreefall || current_mode_ == AltitudeModeFreefallBegin) {
        return true;
    }
    for (uint8_t i = 0; i < GetNumOfPreviousModes(); ++i) {
        if (previous_modes_[i] == AltitudeModePlaneBeforeDrop) {
            return true;
        }
    }

    return false;
}

uint8_t AltitudeManager::GetNumOfPreviousModes()
{
    return 30;
}

void AltitudeManager::CalculateCurrentMode()
{
    uint8_t mode_count[(uint8_t)AltitudeModeNumberOfEnums] = {0,0,0,0,0,0,0,0};
    for (int i = 0; i < GetNumOfPreviousModes(); ++i) {
        mode_count[(uint8_t)previous_modes_[i]]++;
    }

    if (mode_count[(uint8_t)AltitudeModeFreefallOpening]) {
        current_mode_ = AltitudeModeFreefallOpening;
    } else if (mode_count[(uint8_t)AltitudeModeFreefall]) {
        current_mode_ = AltitudeModeFreefall;
    }

    uint8_t maxnum = 0;
    uint8_t idxmax = 0;
    // Skip unused
    for (int i = 1; i < (uint8_t)AltitudeModeNumberOfEnums; ++i) {
        if (mode_count[i] > maxnum) {
            maxnum = mode_count[i];
            idxmax = i;
        }
    }

    current_mode_ = (AltitudeMode)idxmax;
}
