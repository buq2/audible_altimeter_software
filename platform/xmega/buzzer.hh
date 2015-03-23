#ifndef AUDIBLEA_BUZZER_HH
#define AUDIBLEA_BUZZER_HH

#include "axlib/core/io.hh"

class Buzzer
{
 public:
    Buzzer(const axlib::Port buzzer_port,
           const axlib::Pin buzzer_pin);
    void SetFrequency(const uint32_t hz);
    void StepSweep();
 private:
    axlib::Port buzzer_port_;
    axlib::Pin buzzer_pin_;

    uint16_t sweep_freqz_min_;
    uint16_t sweep_freqz_max_;
    uint16_t sweep_freqz_step_;
    uint16_t sweep_current_freqz_;
}; //class Buzzer

#endif //ifndef AUDIBLEA_BUZZER_HH
