#include "buzzer.hh"

Buzzer::Buzzer(const axlib::Port buzzer_port, const axlib::Pin buzzer_pin)
    :
      buzzer_port_(buzzer_port),
      buzzer_pin_(buzzer_pin),
      sweep_freqz_min_(300),
      sweep_freqz_max_(3200),
      sweep_freqz_step_(200),
      sweep_current_freqz_(0)
{
    // Set pin as output
    PORT_t *port = GetPort(buzzer_port);
    port->DIRSET = (int)buzzer_pin;

    SetFrequency(600);
}

void Buzzer::SetFrequency(const uint32_t hz)
{
    TC1_t *tc = GetTimerCounter1(buzzer_port_);

    const uint32_t prescale = 4;
    const uint32_t per = ((F_CPU)/(prescale*hz)+1);

    tc->PER = per;
    tc->CTRLB |= 0x03; //Single slope mode
    tc->CTRLB |= 0b10000; //channel selection CCAEN
    tc->CTRLA |= TC_CLKSEL_DIV4_gc; //clock selection
    tc->CCABUF = per/2; //set the duty cycle as 50%
    while((tc->INTFLAGS & 0x01) == 0);
    tc->INTFLAGS = 0x00; //clear the interrupt flag
}

void Buzzer::StepSweep()
{
    sweep_current_freqz_ = sweep_current_freqz_ - sweep_freqz_step_;
    if (sweep_current_freqz_ <= sweep_freqz_min_) {
        sweep_current_freqz_ = sweep_freqz_max_;
    }
    SetFrequency(sweep_current_freqz_);
}
