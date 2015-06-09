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

BuzzerSound::BuzzerSound()
    :
      active_(false),
      priority_(SoundPriorityLow),
      type_(SoundTypeOff),
      frequency_(0),
      sweep_freqz_min_(300),
      sweep_freqz_max_(6200),
      sweep_freqz_step_(-150),
      num_beeps_left_(0),
      beep_half_period_length_(10),
      beep_period_position_(0),
      beep_frequency_(3200)
{
}

void BuzzerSound::ReplaceIfHigherPriorityOrActive(const BuzzerSound replacement)
{
//    *this = replacement;
    if (!active_) {
        *this = replacement;
    } else if (priority_ <= replacement.priority_) {
        *this = replacement;
    }
}

uint16_t BuzzerSound::StepAndGetFrequency()
{
    if (!active_) {
        return 0;
    }

    switch(type_) {
    case SoundTypeBeep:
        return StepBeep();
    case SoundTypeSweep:
        return StepSweep();
    case SoundTypeOff:
    default:
        return 0;
    }
}

void BuzzerSound::SetVolume(uint8_t volume)
{
    volume_ = volume;
}

uint8_t BuzzerSound::GetVolume()
{
    return volume_;
}

void BuzzerSound::SetPriority(const BuzzerSound::SoundPriority priority)
{
    priority_ = priority;
}

void BuzzerSound::SetType(const BuzzerSound::SoundType type)
{
    type_ = type;
}

void BuzzerSound::SetBeepHalfPeriod(const uint8_t half_period)
{
    beep_half_period_length_ = half_period;
}

BuzzerSound BuzzerSound::Sweep(const BuzzerSound::SoundPriority priority)
{
    BuzzerSound sound;
    sound.type_ = SoundTypeSweep;
    sound.priority_ = priority;
    sound.active_ = true;
    return sound;
}

BuzzerSound BuzzerSound::ConstantBeeping(const uint8_t hz, const BuzzerSound::SoundPriority priority)
{
    BuzzerSound sound;
    sound.type_ = SoundTypeBeep;
    sound.beep_half_period_length_ = 50/hz; //50 comes from 1/(10ms*2)
    sound.priority_ = priority;
    sound.num_beeps_left_ = 255; // 255 beeps
    sound.active_ = true;
    return sound;
}

void BuzzerSound::SetActive(const bool active)
{
    active_ = active;
}

BuzzerSound BuzzerSound::Beep(const uint8_t num_beeps, const BuzzerSound::SoundPriority priority)
{
    BuzzerSound sound;
    sound.type_ = SoundTypeBeep;
    sound.num_beeps_left_ = num_beeps;
    sound.priority_ = priority;
    sound.active_ = true;
    return sound;
}

uint16_t BuzzerSound::StepSweep()
{
    frequency_ = frequency_ + sweep_freqz_step_;
    if (frequency_ <= sweep_freqz_min_ || frequency_ > sweep_freqz_max_) {
        if (sweep_freqz_step_ > 0) {
            frequency_ = sweep_freqz_min_;
        } else {
            frequency_ = sweep_freqz_max_;
        }
    }
    return frequency_;
}

uint16_t BuzzerSound::StepBeep()
{
    if (0 == num_beeps_left_) {
        // No more beeping
        active_ = false;
        return 0;
    }

    if (beep_period_position_ == 0) {
        // At the beginning of period. Change beep on or off

        if (frequency_ == 0) {
            // Set beep frequency
            frequency_ = beep_frequency_;
        } else {
            // Set beep off, reduce beep counter
            frequency_ = 0;
            if (num_beeps_left_ != 255) {
                // 255 signifies constant beeping
                --num_beeps_left_;
            }
        }
    } else {
        // Not at end of beep period
        // Keep current frequency
    }

    ++beep_period_position_;
    beep_period_position_ %= beep_half_period_length_;
    return frequency_;
}
