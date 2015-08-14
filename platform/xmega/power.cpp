#include "power.hh"

#include <avr/power.h>
#include <avr/sleep.h>



void TestSleep()
{
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_mode();
}
