#ifndef ALTITUDE_MANAGER_HH
#define ALTITUDE_MANAGER_HH

struct Config;
class Sensors;
#include <stdint.h>
#include "buzzer_sound.hh"

class AltitudeManager
{
 public:
    typedef enum
            : char
    {
        AltitudeModeUnused = 0,
        AltitudeModePlane = 1,
        AltitudeModePlaneBeforeDrop = 2,
        AltitudeModeFreefallBegin = 3,
        AltitudeModeFreefall = 4,
        AltitudeModeFreefallOpening = 5,
        AltitudeModeCanopy = 6,
        AltitudeModeGround = 7,
        AltitudeModeNumberOfEnums = 8
    } AltitudeMode;

    typedef enum
            : char
    {
        AltitudeModeSimplePlane = 1,
        AltitudeModeSimpleFreefall = 2,
        AltitudeModeSimpleCanopy = 3,
        AltitudeModeSimpleGround = 4
    } AltitudeModeSimple;

    AltitudeManager(Sensors *sensors, Config *config);

    /// \return Current mode of the altimeter
    AltitudeMode GetCurrentMode();

    /// \return Current mode of the altimeter
    AltitudeModeSimple GetCurrentModeSimple();

    /// This functions should be called once 200ms
    void Tick();

    typedef void(*play_sound_fun)(BuzzerSound);
    void SetPlaySoundFunction(play_sound_fun fun);
 private:
    /// \return True if the altimeter is in a plane
    bool InPlane();

    /// \return True if the altimeter might be in freefall
    bool MaybeInFreefall();

    /// \return Number of previous remembered modes
    uint8_t GetNumOfPreviousModes();

    /// Calculate new mode
    void CalculateCurrentMode();

    /// Function which should be called when mode is changed from
    /// freefall to canopy
    void ChangeFromFreeFallToCanopy();

    /// Check if alarms should be sounded
    void CheckForAlarms();

    /// Check freefall alarms
    void CheckFreefallAlarms();

    /// Check canopy alarms
    void CheckCanopyAlarms();

    /// Check plane alarms
    void CheckPlaneAlarms();

    /// Create a sound
    void SetSound(const BuzzerSound sound);

    /// Check for altimeter mode changes
    void CheckForModeChange();
 private:
    // Function pointer which is called when sound should be played
    play_sound_fun play_sound_function_;

    Sensors *sensors_;
    Config *config_;

    // Current mode of the altimeter
    AltitudeMode current_mode_;

    // Previous modes of the altimeter
    AltitudeMode previous_modes_[30];

    // Next free index in previous_modes_
    uint8_t next_mode_index_;
}; //class AltitudeManager

#endif //ifndef ALTITUDE_MANAGER_HH
