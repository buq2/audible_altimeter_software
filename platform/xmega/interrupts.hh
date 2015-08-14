#ifndef AUDIBLEA_INTERRUPTS_HH
#define AUDIBLEA_INTERRUPTS_HH

#include <avr/interrupt.h>

ISR(TCE0_OVF_vect);
ISR(PORTB_INT0_vect);
ISR(PORTD_INT0_vect);
ISR(RTC_OVF_vect);
ISR(TCD0_OVF_vect);

// Seems to be required, as otherwise actual interrupt
// functions will be optimized out of the binary
extern void InitializeInterrupts();

#endif //ifndef AUDIBLEA_INTERRUPTS_HH
