#ifndef GPS_SIM33ELA_HH
#define GPS_SIM33ELA_HH

#include <stdint.h>

class GpsSim33Ela
{
 public:
    GpsSim33Ela();
    uint8_t Setup();
    uint8_t ReceiveData(char *str[]);
    uint8_t SendCommand();
 private:
    char receive_buffer_[256];
}; //class GpsSim33Ela

#endif //ifndef GPS_SIM33ELA_HH
