#include "timer.hh"

#include <avr/io.h>
#include <avr/interrupt.h>

int16_t timer_counter_ = 0;

//ISR(TCE0_OVF_vect)
//ISR(RTC_COMP_vect)




Timer::Timer()
    :
      next_free_(0),
      counter_start_value_(0),
      first_call_(true)
{
//    TCE0.CTRLA = TC_CLKSEL_DIV64_gc;
//    TCE0.PER = 1000;
//    TCE0.INTCTRLA |= TC_OVFINTLVL_LO_gc;
//    TCE0.CNT = 0;



    for (uint8_t i = 0; i < 16; ++i) {
        second_lenghts_[i] = 0;
    }
}

void Timer::TickApproxOneSecond()
{
    if (first_call_) {
        timer_counter_ = 0;
        first_call_ = false;
        return;
    }
    second_lenghts_[next_free_] = timer_counter_;
    counter_start_value_ -= timer_counter_;
    timer_counter_ = 0;
    ++next_free_;
    next_free_ %= 16;
}

void Timer::Tic()
{
    counter_start_value_ = timer_counter_;
}

float Timer::Toc()
{
    return (timer_counter_-counter_start_value_)/(float)GetCountsInSecond();
}

uint16_t Timer::GetCountsInSecond()
{
    uint16_t mmin = 65000;
    for (uint8_t i = 0; i < 16; ++i) {
        if (second_lenghts_[i] > 0 && second_lenghts_[i] < mmin) {
            mmin = second_lenghts_[i];
        }
    }

    return mmin;
}

uint16_t Timer::GetInternalCounter()
{
    return timer_counter_;
}
