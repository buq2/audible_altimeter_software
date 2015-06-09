#include "altitude_manager.hh"
#include "axlib/core/core.hh"
#include "sensors.hh"
#include "config.hh"


AltitudeManager::AltitudeManager(Sensors *sensors, Config *config)
    :
      play_sound_function_(0),
      sensors_(sensors),
      config_(config),
      current_mode_(AltitudeModeGround),
      next_mode_index_(0)
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
    const float altitude_m = sensors_->GetAltitudeMeters();
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

    CheckForModeChange();
    CheckForAlarms();
}

void AltitudeManager::SetPlaySoundFunction(AltitudeManager::play_sound_fun fun)
{
    play_sound_function_ = fun;
}

void AltitudeManager::CheckForModeChange()
{
    static AltitudeMode prev_mode = AltitudeModeGround;
    CalculateCurrentMode();
    const bool change = prev_mode != current_mode_;

    if (!change) {
        return;
    }

    if (current_mode_ == AltitudeModeCanopy) {
        ChangeFromFreeFallToCanopy();
    }

    // Beep to aknowledge the change in mode
    BuzzerSound sound = BuzzerSound::Beep(2);
    sound.SetVolume(GetVolume(Config::AltitudeAlarm::AlarmAmplitudeMedium));;
    sound.SetBeepHalfPeriod(50); // Quite long beeps
    sound.SetPriority(BuzzerSound::SoundPriorityMedium);
    SetSound(sound);
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
        return;
    } else if (mode_count[(uint8_t)AltitudeModeFreefall] > GetNumOfPreviousModes()/3) {
        current_mode_ = AltitudeModeFreefall;
        return;
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

void AltitudeManager::ChangeFromFreeFallToCanopy()
{
    // Mute freefall alarms
    BuzzerSound sound;
    sound.SetPriority(BuzzerSound::SoundPriorityHigh);
    sound.SetType(BuzzerSound::SoundTypeOff);
    sound.SetActive(true);
    SetSound(sound);
}

void AltitudeManager::CheckForAlarms()
{
    const AltitudeModeSimple mode = GetCurrentModeSimple();
    if (mode == AltitudeModeSimpleFreefall) {
        CheckFreefallAlarms();
    } else if (mode == AltitudeModeSimpleCanopy) {
        CheckCanopyAlarms();
    } else if (mode == AltitudeModeSimplePlane) {
        CheckPlaneAlarms();
    }
}

void AltitudeManager::CheckFreefallAlarms()
{
    static float prev_altitude = 0;
    const float altitude_m = sensors_->GetAltitudeMeters();

    const uint8_t num_alarms = sizeof(config_->beeper.alarms_freefall)/sizeof(Config::AltitudeAlarm);
    int16_t lowest_altitude = INT16_MAX;
    BuzzerSound sound;
    for (uint8_t i = 0; i < num_alarms; ++i) {
        const auto &alarm = config_->beeper.alarms_freefall[i];
        if (alarm.enabled && lowest_altitude > alarm.altitude) {
            lowest_altitude = alarm.altitude;

            if (alarm.altitude > prev_altitude || alarm.altitude < altitude_m ) {
                // This alarm has already been triggered, or it should not be
                // triggered yet
                continue;
            }

            switch(alarm.type) {
            case Config::AltitudeAlarm::AlarmTypeFreefallBeeps:
                sound = BuzzerSound::Beep(3, BuzzerSound::SoundPriorityMedium);
                break;
            default:
            case Config::AltitudeAlarm::AlarmTypeLastChange:
                sound = BuzzerSound::Sweep(BuzzerSound::SoundPriorityHigh);
                break;
            }

            sound.SetVolume(GetVolume(alarm.amplitude));
        }
    }
    SetSound(sound);
    prev_altitude = altitude_m;
}

void AltitudeManager::CheckCanopyAlarms()
{
    const float altitude_m = sensors_->GetAltitudeMeters();

    const uint8_t num_alarms = sizeof(config_->beeper.alarms_canopy)/sizeof(Config::AltitudeAlarm);
    int16_t lowest_altitude = INT16_MAX;
    BuzzerSound sound;
    for (uint8_t i = 0; i < num_alarms; ++i) {
        const auto &alarm = config_->beeper.alarms_canopy[i];
        if (alarm.enabled && lowest_altitude > alarm.altitude) {
            lowest_altitude = alarm.altitude;

            switch(alarm.type) {
            default:
                // Not implemented
                break;
            }

            if (alarm.altitude+30 < altitude_m) {
                // This alarm is not yet triggered
                continue;
            }

            if (alarm.altitude > altitude_m) {
                // This alarm has already been triggered
                // Replace with silence
                sound = BuzzerSound();
                sound.SetPriority(BuzzerSound::SoundPriorityMedium);
                sound.SetType(BuzzerSound::SoundTypeOff);
                sound.SetActive(true);
                continue;
            }

            const uint8_t diff_meters = altitude_m-alarm.altitude;
            sound = BuzzerSound::ConstantBeeping(30-diff_meters, BuzzerSound::SoundPriorityMedium);

            sound.SetVolume(GetVolume(alarm.amplitude));
        }
    }
    SetSound(sound);
}

void AltitudeManager::CheckPlaneAlarms()
{
    static float prev_altitude = 0;
    const float altitude_m = sensors_->GetAltitudeMeters();

    if (config_->beeper.climb_altitude.enabled && altitude_m >= 500 && prev_altitude < 500) {
        // Give 500m beep
        BuzzerSound sound = BuzzerSound::Beep(2);

        sound.SetVolume(GetVolume(config_->beeper.climb_altitude.amplitude));

        sound.SetBeepHalfPeriod(50); // Quite long beeps
        SetSound(sound);
    }
    prev_altitude = altitude_m;
}

void AltitudeManager::SetSound(const BuzzerSound sound)
{
    if (0 != play_sound_function_) {
        play_sound_function_(sound);
    }
}
