#include "power.hh"

#include <avr/power.h>
#include <avr/sleep.h>

Power::Power()
{
    //SLEEP_SMODE_ESTDBY_gc almost works (6.8ma)
    //SLEEP_SMODE_PSAVE_gc almost works (5.5ma)
    //SLEEP_SMODE_STDBY_gc does not work
    //SLEEP_SMODE_IDLE_gc works fully (20ma)
    set_sleep_mode(SLEEP_SMODE_IDLE_gc);
    sleep_enable();
}

void Power::MainLoop()
{
    // Withoutusing sleep 28mA
    sleep_cpu();
}
