#include "altimeter_ms5805_02ba01.hh"
#include <LUFA/Drivers/Peripheral/TWI.h>
#include "common.hh"

#define I2C_PORT TWIC
#define I2C_SPEED 400000
#define I2C_TIMEOUT 20
#define MS5805_ADDRESS_READ 0b11101101
#define MS5805_ADDRESS_WRITE 0b11101100

#define RETURN_ERROR_IF_ERROR(x) {if (x) {return x;}}

AltimeterMS5805_02BA01::AltimeterMS5805_02BA01()
    :
      ratio_(OversampleRatio256)
{

}

uint8_t AltimeterMS5805_02BA01::Setup()
{
    // Initialize the TWI driver
    TWI_Init(&I2C_PORT, TWI_BAUD_FROM_FREQ(I2C_SPEED));

    // 0) Reset device
    uint8_t address = 0x1E;
    uint8_t packet = 0;
    uint8_t err = TWI_WritePacket(&I2C_PORT, MS5805_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                                  &packet, 1);
    RETURN_ERROR_IF_ERROR(err);

    // Wait a little (might not be needed)
    _delay_ms(3);

    // 1) Read coefficients
    uint16_t crc = 0;
    err = ReadCoefficient(0, &crc);
    RETURN_ERROR_IF_ERROR(err);
    for (uint8_t i = 1; i < 7; ++i) {
        err = ReadCoefficient(i, &coefficients_[i-1]);
        RETURN_ERROR_IF_ERROR(err);
    }

    return 0;
}

uint8_t AltimeterMS5805_02BA01::GetAltitudeMeters(float *altitude)
{
    return 0;
}

uint8_t AltimeterMS5805_02BA01::GetTemperatureCelcius(float *temp)
{
    uint32_t d1, d2;

    uint8_t err = ReadMeasurement(RawValueD1,ratio_, &d1);
    RETURN_ERROR_IF_ERROR(err);

    err = ReadMeasurement(RawValueD2,ratio_, &d2);
    RETURN_ERROR_IF_ERROR(err);

    const int64_t dt = d2-coefficients_[5-1]*255;
    const int64_t temperature = 2000+(dt*coefficients_[6-1])/pow(2,23);
    *temp = temperature/100.0f;

    const int64_t offset = coefficients_[2-1]*pow(2,17)+dt*coefficients_[4-1]/pow(2,6); ;//coefficients_[2-1]*pow(2,17)+(coefficients_[4-1]*dt)/pow(2,6);
    const int64_t sens = coefficients_[1-1]*pow(2,16)+(coefficients_[3-1]*dt)/pow(2,7);
    const int64_t pressure = ((d1*sens)/pow(2,21)-offset)/pow(2,15);

    //*temp = pressure/100.0f;

    return 0;
}

void AltimeterMS5805_02BA01::SetOversampleRatio(const AltimeterMS5805_02BA01::OversampleRatio ratio)
{
    ratio_ = ratio;
}

uint8_t AltimeterMS5805_02BA01::ReadCoefficient(const uint8_t coefnum, uint16_t *coef)
{
    uint8_t address = 0xA0 + coefnum*2;
    uint8_t err = TWI_ReadPacket(&I2C_PORT, MS5805_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                                  (uint8_t*)coef, 2);
    return err;
}

uint8_t AltimeterMS5805_02BA01::ReadMeasurement(const AltimeterMS5805_02BA01::RawValue d,
                                                const AltimeterMS5805_02BA01::OversampleRatio ratio,
                                                uint32_t *value)
{
    uint8_t address = 0b01000000;
    if (d == RawValueD2) {
        address = BIT_SET(address,4);
    }
    address += ((uint8_t)ratio)*2;

    // Initiate conversion
    uint8_t err = TWI_ReadPacket(&I2C_PORT, MS5805_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                                  (uint8_t*)value, 0);

    // We must sleep appropriate time, otherwise reading will fail
    switch(ratio) {
    case OversampleRatio256:
        _delay_us(900);
        break;
    case OversampleRatio512:
        _delay_ms(3);
        break;
    case OversampleRatio1024:
        _delay_ms(4);
        break;
    case OversampleRatio2048:
        _delay_ms(6);
        break;
    case OversampleRatio4096:
        _delay_ms(10);
        break;
    case OversampleRatio8192:
        _delay_ms(19);
        break;
    }

    // Get value
    address = 0;
    err = TWI_ReadPacket(&I2C_PORT, MS5805_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                                  (uint8_t*)value, 3);

    return err;
}
