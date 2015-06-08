#ifndef ALTITUDE_MANAGER_HH
#define ALTITUDE_MANAGER_HH

struct Config;
class Sensors;
#include <stdint.h>

class AltitudeManager
{
 public:
    typedef enum
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
    {
        AltitudeModeSimplePlane = 1,
        AltitudeModeSimpleFreefall = 2,
        AltitudeModeSimpleCanopy = 3,
        AltitudeModeSimpleGround = 4
    } AltitudeModeSimple;

    AltitudeManager(Sensors *sensors, Config *config);
    AltitudeMode GetCurrentMode();
    AltitudeModeSimple GetCurrentModeSimple();
    void Tick200ms();
 private:
    bool InPlane();
    bool MaybeInFreefall();
    uint8_t GetNumOfPreviousModes();
    void CalculateCurrentMode();
 private:
    Sensors *sensors_;
    Config *config_;
    AltitudeMode current_mode_;

    AltitudeMode previous_modes_[30];
    uint8_t next_mode_index_;
    uint16_t freefall_counter_;
}; //class AltitudeManager

#endif //ifndef ALTITUDE_MANAGER_HH
