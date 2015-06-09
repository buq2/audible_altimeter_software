#ifndef AUDIBLEA_TIMER_HH
#define AUDIBLEA_TIMER_HH

#include "axlib/core/io.hh"

/// Relatively simple timer for timing how long operations take
class Timer
{
 public:
    Timer();

    /// This function should be called once a second. Used for calibrating
    void TickApproxOneSecond();

    /// Start counting event length
    void Tic();

    /// \return Time in seconds from Tic()
    float Toc();

    /// Get number of counts in one second
    uint16_t GetCountsInSecond();

    /// Get internal counter value
    uint16_t GetInternalCounter();
 private:

 private:
    // Lengths of seconds in internal counter values
    int16_t second_lenghts_[16];

    // Next free index in second_lengths_
    uint8_t next_free_;

    // Value of the timer counter when Tic() was called
    int16_t counter_start_value_;

    // If true, TickApproxOneSecond has not been called
    bool first_call_;
}; //class Timer

#endif //ifndef AUDIBLEA_TIMER_HH
