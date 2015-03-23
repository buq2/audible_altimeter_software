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

template<class T>
void SetFrequency_int(T *tc, const uint16_t per, const TC_CLKSEL_t clocksel)
{
    tc->PER = per;
    tc->CTRLB |= 0x03; //Single slope mode
    tc->CTRLB |= 0b10000; //channel selection CCAEN
    tc->CTRLA |= clocksel; //clock selection
    tc->CCABUF = per/2; //set the duty cycle as 50%
    while((tc->INTFLAGS & 0x01) == 0);
    tc->INTFLAGS = 0x00; //clear the interrupt flag
}

uint16_t GetPeriod(const uint32_t hz)
{
    const uint32_t prescale = 4;
    return F_CPU/(prescale*hz)+1;
}

TC_CLKSEL_t GetClockSelection()
{
    return TC_CLKSEL_DIV4_gc;
}

void Buzzer::SetFrequency(const uint32_t hz)
{
    if ((int)buzzer_pin_ < (int)axlib::PIN_4) {
        SetFrequency_int(GetTimerCounter0(buzzer_port_), GetPeriod(hz), GetClockSelection());
    } else {
        SetFrequency_int(GetTimerCounter1(buzzer_port_), GetPeriod(hz), GetClockSelection());
    }
}

void Buzzer::StepSweep()
{
    sweep_current_freqz_ = sweep_current_freqz_ - sweep_freqz_step_;
    if (sweep_current_freqz_ <= sweep_freqz_min_) {
        sweep_current_freqz_ = sweep_freqz_max_;
    }
    SetFrequency(sweep_current_freqz_);
}

