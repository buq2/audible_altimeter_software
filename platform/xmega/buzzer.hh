#ifndef AUDIBLEA_BUZZER_HH
#define AUDIBLEA_BUZZER_HH

#include "axlib/core/io.hh"
#include "axlib/sensors/digipot_mcp4017t.hh"

class Buzzer
{
 public:
    Buzzer(const axlib::Port buzzer_port,
           const axlib::Pin buzzer_pin,
           axlib::DigipotMcp4017T *digipot);
    void SetFrequency(const uint32_t hz);
    void Tick10ms();
    void Beep(const uint8_t num_beeps = 1);
 private:
    void StepSweep();
 private:
    axlib::Port buzzer_port_;
    axlib::Pin buzzer_pin_;

    bool sweep_on_;
    uint16_t sweep_freqz_min_;
    uint16_t sweep_freqz_max_;
    uint16_t sweep_freqz_step_;
    uint16_t current_freqz_;

    bool beep_on_;
    uint8_t num_beeps_;

    axlib::DigipotMcp4017T * volume_control_;
}; //class Buzzer

#endif //ifndef AUDIBLEA_BUZZER_HH
