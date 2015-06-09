#include "buzzer_sound.hh"

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
      beep_frequency_(3200),
      volume_(0)
{
}

void BuzzerSound::ReplaceIfHigherPriorityOrActive(const BuzzerSound replacement)
{
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

void BuzzerSound::SetBeepBuzzFrequency(const uint16_t freq)
{
    beep_frequency_ = freq;
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
