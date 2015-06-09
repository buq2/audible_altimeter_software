#ifndef AUDIBLEA_BUZZER_HH
#define AUDIBLEA_BUZZER_HH

#include "axlib/core/io.hh"
#include "axlib/sensors/digipot_mcp4017t.hh"
#include "buzzer_sound.hh"

/// Buzzer controls actual piezo/speaker
class Buzzer
{
 public:
    /// \param[in] buzzer_port Port number of the buzzer
    /// \param[in] buzzer_pin Buzzer drive pin
    /// \param[in] digipot Address of the digipot for controlling the volume
    Buzzer(const axlib::Port buzzer_port,
           const axlib::Pin buzzer_pin,
           axlib::DigipotMcp4017T *digipot);

    /// Set speaker drive frequency
    /// \param[in] hz Frequency to be set
    void SetFrequency(const uint32_t hz);

    /// function which should be called 100 times a second
    void Tick10ms();

    /// Create simple beep
    /// \param[in] num_beeps Number of beeps to create
    void Beep(const uint8_t num_beeps = 1);

    /// Set current sound
    void SetSound(const BuzzerSound sound);
 private:
    /// Set volume / dipot value
    /// \param[in] Volume.
    void SetVolume(const uint8_t volume);
 private:
    // Buzzer pin port
    axlib::Port buzzer_port_;

    // Buzzer pin
    axlib::Pin buzzer_pin_;

    // Digital pot used for volume control
    axlib::DigipotMcp4017T * volume_control_;

    // Current frequency of the buzzer pin
    uint16_t current_freqz_;

    // Current volume
    uint8_t current_volume_;

    BuzzerSound current_sound_;
}; //class Buzzer

#endif //ifndef AUDIBLEA_BUZZER_HH
