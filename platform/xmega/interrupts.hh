#ifndef AUDIBLEA_INTERRUPTS_HH
#define AUDIBLEA_INTERRUPTS_HH

#include <avr/interrupt.h>

ISR(TCE0_OVF_vect);
ISR(PORTB_INT0_vect);
ISR(PORTD_INT0_vect);
ISR(RTC_OVF_vect);
ISR(TCD0_OVF_vect);

#endif //ifndef AUDIBLEA_INTERRUPTS_HH
