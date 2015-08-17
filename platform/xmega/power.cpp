#include "power.hh"

#include <avr/power.h>
#include <avr/sleep.h>
#include "components.hh"

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
    Components *comp = GetComponents();

    const bool buttons_ok = comp->GetButtons()->AllowDeepSleep();
    const bool buzzer_ok = comp->GetBuzzer()->AllowDeepSleep();

    if (buttons_ok && buzzer_ok) {
        set_sleep_mode(SLEEP_SMODE_PSAVE_gc);
    } else {
        set_sleep_mode(SLEEP_SMODE_IDLE_gc);
    }

    // Withoutusing sleep 28mA
    sleep_cpu();
}
