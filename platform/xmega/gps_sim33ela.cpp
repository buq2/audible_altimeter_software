#include "gps_sim33ela.hh"
#include <LUFA/Drivers/Peripheral/Serial.h>

#define SERIAL_PORT USARTE0
#define SERIAL_PORT_ALPHA PORTE
#define SERIAL_SPEED 9600

GpsSim33Ela::GpsSim33Ela()
{
    receive_buffer_[256] = 0;
}

uint8_t GpsSim33Ela::Setup()
{
    SERIAL_PORT_ALPHA.DIRSET = 1<<3;
    SERIAL_PORT_ALPHA.DIRCLR = 1<<2;
    SERIAL_PORT_ALPHA.OUTCLR = 1<<3;

    const bool double_speed = false;
    Serial_Init(&SERIAL_PORT, SERIAL_SPEED, double_speed);
    return 0;
}

uint8_t GpsSim33Ela::ReceiveData(char *str[])
{
    *str = receive_buffer_;

    uint8_t i = 0;
    while (true) {
        int16_t byte = Serial_ReceiveByte(&SERIAL_PORT);

        if (byte < 0) {
            // No data, wait
            continue;
        }

        if (byte == '\n') {
            // Lines end with '\n'
            receive_buffer_[i] = byte;
            receive_buffer_[++i] = 0;
            return 0;
        } else {
            receive_buffer_[i] = byte;
        }
        ++i;
    }

    return 0;
}

uint8_t GpsSim33Ela::SendCommand()
{
    Serial_SendString(&SERIAL_PORT, "PSIMIPR,T\r\n");
    //Serial_SendString(&SERIAL_PORT, "$PMTK000*32\r\n");
    return 0;
}
