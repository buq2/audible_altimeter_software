#ifndef AUDIBLEA_TIMER_HH
#define AUDIBLEA_TIMER_HH

#include "axlib/core/io.hh"

class Timer
{
 public:
    Timer();
    void TickApproxOneSecond();
    void Tic();
    float Toc();
    uint16_t GetCountsInSecond();
 private:

 private:
    int16_t second_lenghts_[16];
    uint8_t next_free_;
    int16_t counter_start_value_;
    bool first_call_;
}; //class Timer

#endif //ifndef AUDIBLEA_TIMER_HH
