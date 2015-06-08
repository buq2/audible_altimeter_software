#include "buzzer.hh"
#include <avr/interrupt.h>

uint16_t GetPeriod(const TC_CLKSEL_t prescale_enum, const uint32_t hz)
{
    uint32_t prescale;
    switch(prescale_enum) {
    case TC_CLKSEL_DIV1_gc:
        prescale = 1;
        break;
    case TC_CLKSEL_DIV2_gc:
        prescale = 2;
        break;
    case TC_CLKSEL_DIV4_gc:
        prescale = 4;
        break;
    case TC_CLKSEL_DIV8_gc:
        prescale = 8;
        break;
    case TC_CLKSEL_DIV64_gc:
        prescale = 64;
        break;
    case TC_CLKSEL_DIV256_gc:
        prescale = 256;
        break;
    case TC_CLKSEL_DIV1024_gc:
    default:
        prescale = 1024;
        break;
    }
    return F_CPU/(prescale*hz)+1;
}

TC_CLKSEL_t GetClockSelection()
{
    return TC_CLKSEL_DIV4_gc;
}

template<class T>
void SetFrequency_int(T *tc, const uint32_t hz, const TC_CLKSEL_t clocksel)
{
    const uint16_t per = GetPeriod(clocksel, hz);
    tc->PER = per;
    tc->CTRLB |= 0x03; //Single slope mode
    tc->CTRLB |= 0b10000; //channel selection CCAEN
    tc->CTRLA |= clocksel; //clock selection
    tc->CCABUF = per/2; //set the duty cycle as 50%
    while((tc->INTFLAGS & 0x01) == 0);
    tc->INTFLAGS = 0x00; //clear the interrupt flag
}

Buzzer *global_buzzer_  = 0;


// This should be made from main_mcu.cpp
void SetBuzzerModulationTC(Buzzer *buzzer)
{
    global_buzzer_ = buzzer;
    GetTimerCounter0(axlib::PORT_E)->INTCTRLA |= TC_OVFINTLVL_LO_gc;
    SetFrequency_int(GetTimerCounter0(axlib::PORT_E), 100, TC_CLKSEL_DIV256_gc);
}

ISR(TCE0_OVF_vect)
{
    global_buzzer_->Tick10ms();
}


Buzzer::Buzzer(const axlib::Port buzzer_port, const axlib::Pin buzzer_pin, axlib::DigipotMcp4017T *digipot)
    :
      buzzer_port_(buzzer_port),
      buzzer_pin_(buzzer_pin),
      sweep_freqz_min_(300),
      sweep_freqz_max_(6200),
      sweep_freqz_step_(150),
      current_freqz_(0),
      volume_control_(digipot),
      beep_on_(false),
      sweep_on_(false),
      num_beeps_(0)
{
    // Set pin as output
    PORT_t *port = GetPort(buzzer_port);
    port->DIRSET = (int)buzzer_pin;
    SetBuzzerModulationTC(this);
    volume_control_->SetValue(0);
}

void Buzzer::SetFrequency(const uint32_t hz)
{
    if (current_freqz_ == hz) {
        return;
    }
    if ((int)buzzer_pin_ < (int)axlib::PIN_4) {
        SetFrequency_int(GetTimerCounter0(buzzer_port_), hz, GetClockSelection());
    } else {
        SetFrequency_int(GetTimerCounter1(buzzer_port_), hz, GetClockSelection());
    }
    current_freqz_ = hz;
}

void Buzzer::StepSweep()
{
    uint16_t new_freq = current_freqz_ - sweep_freqz_step_;
    if (new_freq <= sweep_freqz_min_ || new_freq > sweep_freqz_max_) {
        new_freq = sweep_freqz_max_;
    }
    SetFrequency(new_freq);
}

void Buzzer::Tick10ms()
{
    if (num_beeps_ == 0 && !sweep_on_) {
        SetFrequency(0);
    } else if (sweep_on_) {
        StepSweep();
    } else if (num_beeps_ > 0) {
        --num_beeps_;
        if (beep_on_) {
            SetFrequency(3200);
        } else {
            SetFrequency(0);
        }
        beep_on_ = !beep_on_;
    }
}

void Buzzer::Beep(const uint8_t num_beeps)
{
    beep_on_ = true;
    num_beeps_ += num_beeps;
}

