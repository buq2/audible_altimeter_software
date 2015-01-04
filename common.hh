#ifndef COMMON_HH
#define COMMON_HH

#include <stdint.h>

#ifdef USE_MCU
#include <avr/pgmspace.h>
#else
#define PROGMEM
#define pgm_read_byte(x) (*(x))
#endif

#ifndef MIN
#define MIN(x,y)((x)<(y)?(x):(y))
#endif

#ifndef MAX
#define MAX(x,y)((x)>(y)?(x):(y))
#endif

#ifndef ABS
#define ABS(x)((x)<0?(-(x)):(x))
#endif

#ifndef NULL
#define NULL 0
#endif

extern uint8_t BitFlip(uint8_t n);

#endif //ifndef COMMON_HH
