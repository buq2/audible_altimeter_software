#ifndef BUZZER_SOUND_HH
#define BUZZER_SOUND_HH

#include <stdint.h>

class BuzzerSound
{
 public:
    typedef enum {
        SoundTypeOff,
        SoundTypeBeep,
        SoundTypeSweep
    } SoundType;

    typedef enum {
        SoundPriorityLow = 0,
        SoundPriorityMedium = 1,
        SoundPriorityHigh = 2,
    } SoundPriority;

    BuzzerSound();

    /// Replace data in this if replacement has same or higher priority
    /// or if this is not enabled
    void ReplaceIfHigherPriorityOrActive(const BuzzerSound replacement);

    /// Step/tick the sound and get next frequency to be set to the buzzer
    uint16_t StepAndGetFrequency();

    /// Set volume for the sound
    /// \param[in] volume Volume. 0 = quiet, 255 = loud
    void SetVolume(uint8_t volume);

    /// \return Volume for the sound
    uint8_t GetVolume();

    /// Set priority
    /// \param[in] priority Priority for this sound
    void SetPriority(const SoundPriority priority);

    /// Set type of the sound
    /// \param[in] type Type of the sound
    void SetType(const SoundType type);

    /// Set beep hald period
    /// \param[in] half_period Longer periods give slower beeps
    void SetBeepHalfPeriod(const uint8_t half_period);

    /// \param[in] num_beeps Number of beeps to generate
    /// \param[in] priority Priority of the beeps
    /// \return Beep
    static BuzzerSound Beep(const uint8_t num_beeps = 1,
                            const SoundPriority priority = SoundPriorityLow);

    /// Create sweep sound
    /// \param[in] priority Priority of the sweep
    /// \return Sweep sound
    static BuzzerSound Sweep(const SoundPriority priority = SoundPriorityHigh);

    /// Create constant beeping
    /// \parma[in] hz Aprroximate frequency of the beeping (1-50)
    /// \param[in] priority Priority of the beeping
    /// \return Constant beeping
    static BuzzerSound ConstantBeeping(const uint8_t hz,
                                       const SoundPriority priority = SoundPriorityMedium);

    /// Set if the sound is active (playing) or not
    /// \param[in] active If true, sound could be played
    void SetActive(const bool active);

    /// \return True if active (playing)
    bool IsActive() const;

    /// Set beep buzz frequency
    void SetBeepBuzzFrequency(const uint16_t freq);
 private:
    /// Step sweep and
    /// \return New frequency for the sound
    uint16_t StepSweep();

    /// Step beep and
    /// \return New frequency for the sound
    uint16_t StepBeep();
 private:
    // True if the sound is still active (/playing)
    bool active_;

    // Priority of the sound
    SoundPriority priority_;

    // Type of the sound
    SoundType type_;

    // Volume of the sound (0=quiet, 255=loud)
    uint8_t volume_;

    // Current frequency of the sound
    uint16_t frequency_;

    // Sweep related
    // Minimum frequency of the sweep sound in Hz
    uint16_t sweep_freqz_min_;
    // Maximum frequency of the sweep sound in Hz
    uint16_t sweep_freqz_max_;
    // Frequency step of the sweep in Hz
    int16_t sweep_freqz_step_;

    // Beep related
    // How many beeps are left
    uint8_t num_beeps_left_;
    // Length of beep on/off period
    uint8_t beep_half_period_length_;
    // At what position of th ebeep period we are currently
    uint8_t beep_period_position_;
    // Frequency of the beep
    uint16_t beep_frequency_;
};

#endif //ifndef BUZZER_SOUND_HH
