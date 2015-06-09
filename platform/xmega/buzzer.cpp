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
      volume_control_(digipot),
      current_freqz_(0)
{
    // Set pin as output
    PORT_t *port = GetPort(buzzer_port);
    port->DIRSET = (int)buzzer_pin;
    SetBuzzerModulationTC(this);
    SetVolume(255);
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

void Buzzer::Tick10ms()
{
    SetFrequency(current_sound_.StepAndGetFrequency());
}

void Buzzer::Beep(const uint8_t num_beeps)
{
    current_sound_ = BuzzerSound::Beep(num_beeps);
    SetVolume(current_sound_.GetVolume());
}

void Buzzer::SetSound(const BuzzerSound sound)
{
    current_sound_.ReplaceIfHigherPriorityOrActive(sound);
    SetVolume(current_sound_.GetVolume());
}

void Buzzer::SetVolume(const uint8_t volume)
{
    if (current_volume_ == volume) {
        return;
    }
    volume_control_->SetValue(255-volume); //Controls resistance, not volume
    current_volume_ = volume;
}

