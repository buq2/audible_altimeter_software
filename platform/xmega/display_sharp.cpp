#include "display_sharp.hh"
#include <LUFA/Drivers/Peripheral/SPI.h>
#include <util/delay.h>
#include "common.hh"

#define SPI_SS_PIN 0b00010000
#define SPI_MOSI_PIN 0b00100000
#define SPI_SCK_PIN 0b10000000
#define EXTCOMIN_PIN 0b00000100
#define SPI_PORT PORTC
#define SPI_VAR SPIC
#define EXCOMIN_PORT PORTB
#define CS_PORT PORTD
#define CS_PIN 0b00000100;

DisplaySharp::DisplaySharp(const uint8_t width, const uint8_t height)
    :
      width_(width),
      height_(height)
{

}

bool DisplaySharp::Setup()
{
    // Setup SPI pins
    SPI_PORT.DIRSET  = SPI_MOSI_PIN | SPI_SCK_PIN | SPI_SS_PIN;
    SPI_PORT.PIN4CTRL = PORT_OPC_WIREDANDPULL_gc;

    SetChipSelected(0);

    // Set the actual CS pin (which is not in the SPI port)
    CS_PORT.DIRSET = CS_PIN;
    CS_PORT.OUTCLR = CS_PIN;

    // Setup SPI
    // It seems that even SPI_SPEED_FCPU_DIV_4  could be usable
    SPI_Init(&SPI_VAR,
             SPI_SPEED_FCPU_DIV_128  | SPI_ORDER_MSB_FIRST | SPI_SCK_LEAD_RISING |
            SPI_SAMPLE_TRAILING | SPI_MODE_MASTER);

    // Setup VCOM signal
    EXCOMIN_PORT.DIRSET = EXTCOMIN_PIN;
    EXCOMIN_PORT.OUTCLR = EXTCOMIN_PIN;

    return 0;
}

void DisplaySharp::SetContent(const DisplayBuffer &buffer)
{
    if (buffer.GetWidth() != width_ || buffer.GetHeight() != height_) {
        return;
    }

    SetChipSelected(1);
    _delay_us(6);

    SendByte(0b10000000); //Write line command
    for (uint8_t y = 0; y < height_; ++y) {
        const uint8_t *data = buffer.GetBufferRow(y);

        // Command for line modification
        SendByte(BitFlip(y+1));

        for (uint8_t x = 0; x < width_/8; ++x) {
            SendByte(data[x]);
        }

        // 8 clocks of padding
        SendByte(0);
    }
    // Additional 8 clocks of padding
    SendByte(0);

    _delay_us(2);
    SetChipSelected(0);
}

void DisplaySharp::ToggleExtcomin()
{
    static uint8_t comstatus = 0;
    if (comstatus == 0) {
        EXCOMIN_PORT.OUTSET = EXTCOMIN_PIN;
        comstatus = 1;
    } else {
        EXCOMIN_PORT.OUTCLR = EXTCOMIN_PIN;
        comstatus = 0;
    }
}

void DisplaySharp::Clear()
{
    SetChipSelected(1);
    _delay_us(6);

    SendByte(0b00000100);
    SendByte(0);

    _delay_us(2);
    SetChipSelected(0);
}

void DisplaySharp::SetChipSelected(const uint8_t val)
{
    if (val == 0) {
        CS_PORT.OUTCLR = CS_PIN;
    } else {
        CS_PORT.OUTSET = CS_PIN;
    }
}

void DisplaySharp::SendByte(const uint8_t val)
{
    SPI_SendByte(&SPI_VAR, val);
}
