#include "power.hh"

#include <avr/power.h>
#include <avr/sleep.h>
#include "components.hh"
#include <avr/io.h>

Power::Power()
{
    //SLEEP_SMODE_ESTDBY_gc almost works (6.8ma)
    //SLEEP_SMODE_PSAVE_gc almost works (5.5ma)
    //SLEEP_SMODE_STDBY_gc does not work
    //SLEEP_SMODE_IDLE_gc works fully (20ma)
    set_sleep_mode(SLEEP_SMODE_IDLE_gc);
    sleep_enable();

    // Enabled ADC-A
    ADCA.CTRLA = ADC_ENABLE_bm;
    ADCA.CTRLB = ADC_RESOLUTION_12BIT_gc | ADC_CURRLIMIT_HIGH_gc;
    // Use VCC/1.6 as reference and enabled bandgap voltage
    ADCA.REFCTRL = ADC_REFSEL_INTVCC_gc | ADC_BANDGAP_bm;
    // Internal voltages must be sampled at max 12xKHz
    ADCA.PRESCALER = ADC_PRESCALER_DIV512_gc; //(48MHz/512->93Khz)

    // Measure bandgap
    ADCA.CH0.MUXCTRL = ADC_CH_MUXINT_BANDGAP_gc;

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

    // Without using sleep 28mA
    sleep_cpu();
}

float Power::GetVccVoltage()
{
    // Start conversion
    ADCA.CH0.CTRL = ADC_CH_START_bm | ADC_CH_INPUTMODE_INTERNAL_gc;

    // Wait until finished
    while(!(ADCA.CH0.INTFLAGS & ADC_CH_CHIF_bm)) {}
    ADCA.CH0.INTFLAGS = ADC_CH_CHIF_bm; // by setting flag to 1 it is cleared

    // Get 12bit result
    uint16_t res = ADCA.CH0.RES;

    // Convert to VCC
    // The result is calculated as: res = (1.1+0.05*(vcc/1.6))/(vcc/1.6)*2**12
    // -> vcc = 180224/(25*res - 5120)
    return 180224.0f/(25.0f*res-5120.0f);
}
