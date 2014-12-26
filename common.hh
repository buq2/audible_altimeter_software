#ifndef COMMON_HH
#define COMMON_HH

#ifdef USE_MCU
#include <avr/pgmspace.h>
#else
#define PROGMEM
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

#endif //ifndef COMMON_HH
