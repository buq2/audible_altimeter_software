#include "altimeter_mpl3115a2.hh"
#include <LUFA/Drivers/Peripheral/TWI.h>
#include "common.hh"

#define I2C_PORT TWIC
#define I2C_SPEED 400000
#define I2C_TIMEOUT 20
#define MPL3115A2_ADDRESS_READ 0xC1 // 1100 0001
#define MPL3115A2_ADDRESS_WRITE 0xC0 // 1100 0000

#define RETURN_ERROR_IF_ERROR(x) {if (x) {return x;}}
#define RETURN_NULL_IF_ERROR(x) {if (x) {return NULL;}}

AltimeterMPl3114A2::AltimeterMPl3114A2()
    :
      request_data_called_(false),
      configuration_data_(0b10011100),
      altitude_ground_(0.0f)
{
}

uint8_t AltimeterMPl3114A2::Setup()
{
    // Initialize the TWI driver
    TWI_Init(&I2C_PORT, TWI_BAUD_FROM_FREQ(I2C_SPEED));

    uint8_t address = 0x26;
    uint8_t packet = configuration_data_;
    BIT_SET(packet,0);
    uint8_t err = TWI_WritePacket(&I2C_PORT, MPL3115A2_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                                  &packet, 1);
    RETURN_ERROR_IF_ERROR(err);

    address = 0x13;
    err = TWI_WritePacket(&I2C_PORT, MPL3115A2_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                          &configuration_data_, 1);
    RETURN_ERROR_IF_ERROR(err);

    return 0;
}

void AltimeterMPl3114A2::SetMode(const bool altimeter)
{
    if (altimeter) {
        configuration_data_ = BIT_SET(configuration_data_,7);
    } else {
        configuration_data_ = BIT_CLEAR(configuration_data_,7);
    }
}

uint8_t AltimeterMPl3114A2::RequestDataUpdate()
{
    //Set Active
    uint8_t address = 0x26;
    uint8_t err = TWI_WritePacket(&I2C_PORT, MPL3115A2_ADDRESS_WRITE, I2C_TIMEOUT, &address, 1,
                                  &configuration_data_, 1);
    RETURN_ERROR_IF_ERROR(err);
    request_data_called_ = true;

    return 0;
}

uint8_t AltimeterMPl3114A2::DataReady()
{
    // Check status register
    uint8_t address = 0;
    uint8_t packet = 0;
    uint8_t err = TWI_ReadPacket(&I2C_PORT, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                    &packet, 1);
    RETURN_ERROR_IF_ERROR(err);

    // 0x08 is 1 if OUT_P or OUT_T has been overwritten
    // this flag is cleared by reading OUT_P_MSB or OUT_T_MSB
    if (packet & 0x08) {
        // Altitude ready
        return 1;
    }
    return 0;
}

uint8_t AltimeterMPl3114A2::GetAltitudeMeters(float *altitude)
{
    uint8_t err = GetRawAltitudeMeters(altitude);
    RETURN_ERROR_IF_ERROR(err);

    *altitude -= altitude_ground_;

    return 0;
}

uint8_t AltimeterMPl3114A2::GetTemperature(float *temperature)
{
    uint8_t OUT_T_MSB;
    uint8_t OUT_T_LSB;
    uint8_t address = 0x04;
    uint8_t err = TWI_ReadPacket(&I2C_PORT, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                         &OUT_T_MSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    address = 0x05;
    err = TWI_ReadPacket(&I2C_PORT, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                         &OUT_T_LSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    const int16_t temp_raw = OUT_T_LSB | (((uint16_t)OUT_T_MSB)<<8);

    *temperature = temp_raw/256.0f;;

    return 0;
}

uint8_t AltimeterMPl3114A2::GetPressurePascals(float *pressure)
{
    uint8_t OUT_P_MSB;
    uint8_t OUT_P_CSB;
    uint8_t OUT_P_LSB;
    uint8_t address = 0x01;
    uint8_t err = TWI_ReadPacket(&I2C_PORT, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &OUT_P_MSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    address = 0x02;
    err = TWI_ReadPacket(&I2C_PORT, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                     &OUT_P_CSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    address = 0x03;
    err = TWI_ReadPacket(&I2C_PORT, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                     &OUT_P_LSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    const int32_t alt_raw = OUT_P_LSB | (((uint32_t)OUT_P_CSB)<<8) | (((uint32_t)OUT_P_MSB)<<16);

    // Convert to altitude in meters
    *pressure = alt_raw/64.0f;

    return 0;
}

char *AltimeterMPl3114A2::GetAltitudeMetersString()
{
    uint8_t err = 0;
    if (!request_data_called_) {
        err = RequestDataUpdate();
        RETURN_NULL_IF_ERROR(err);
    }

    if (DataReady()) {
        float alt = 0.0f;
        err = GetPressurePascals(&alt);
        RETURN_NULL_IF_ERROR(err);

        sprintf(altitude_string_,"%f\n\r",alt);
        return altitude_string_;
    }

    return NULL;
}

void AltimeterMPl3114A2::ZeroAltitudeLoop()
{
    //while (0 != ZeroAltitude()) {
    //}
}

uint8_t AltimeterMPl3114A2::ZeroAltitude()
{
    return GetRawAltitudeMeters(&altitude_ground_);
}

uint8_t AltimeterMPl3114A2::GetRawAltitudeMeters(float *altitude)
{
    uint8_t OUT_P_MSB;
    uint8_t OUT_P_CSB;
    uint8_t OUT_P_LSB;
    uint8_t address = 0x01;
    uint8_t err = TWI_ReadPacket(&I2C_PORT, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                 &OUT_P_MSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    address = 0x02;
    err = TWI_ReadPacket(&I2C_PORT, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                     &OUT_P_CSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    address = 0x03;
    err = TWI_ReadPacket(&I2C_PORT, MPL3115A2_ADDRESS_READ, I2C_TIMEOUT, &address, 1,
                                     &OUT_P_LSB, 1);
    RETURN_ERROR_IF_ERROR(err);

    const int32_t alt_raw = ((uint32_t)OUT_P_LSB<<8) | (((uint32_t)OUT_P_CSB)<<16) | (((uint32_t)OUT_P_MSB)<<24);

    // Convert to altitude in meters
    *altitude = ((float)alt_raw)/65536.0f;

    return 0;
}
