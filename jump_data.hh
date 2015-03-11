#ifndef AUDIBLEA_JUMP_DATA_HH
#define AUDIBLEA_JUMP_DATA_HH

#include "config.hh"

// First sector (beginning from address 0) contains:
// 1)  configuration data (Config)

// Next sectors will contain jump log data

#define JUMPDATA_HEADER_MAGIC 0xBEEFCA5E

// Stored at the beginning of each 4kb sector
typedef struct JumpData_Header_t
{
    uint32_t magic_number; // Magic number = 0xBEEFCA5E. If sector does not begin with this number -> invalid sector
    uint32_t jump_number; // Starts from 1.
} JumpData_Header;

// After JumpData_Header begins the actual jump data. Before each struct
// we have uint8_t which describes next struct type:

typedef enum JumpData_StructEnum_t
{
    JUMPDATA_STRUCT_ENUM_JUMP_INFO = 1, //JumpData_Date_Time
    JUMPDATA_STRUCT_ENUM_BEGIN_SENSOR_DATA = 2, // JumpData_BeginSensorData
    JUMPDATA_STRUCT_ENUM_ALTITUDE = 3, // JumpData_Altitude
    JUMPDATA_STRUCT_ENUM_ALTITUDE_RATE = 4, // JumpData_AltitudeRate
    JUMPDATA_STRUCT_ENUM_SECONDS_TIC = 5, // JumpData_SecondsTic
} JumpData_StructEnum;

typedef struct JumpData_Info_t
{
    char jump_date_time[20]; //format "2015-03-11 20:23:10\0"
    char dropzone[32]; //freeformat
} JumpData_Date_Info;

typedef struct JumpData_BeginSensorData_t
{
    // Time in seconds since last sensor data update
    float time_since_last_sensor_data_seconds; 
} JumpData_BeginSensorData;

typedef struct JumpData_Altitude_t
{
    uint8_t altitude_sensor_id;
    float altitude_meters;
} JumpData_Altitude;

typedef struct JumpData_AltitudeRate_t
{
    float altitude_rate_m_per_s;
} JumpData_AltitudeRate;

typedef struct JumpData_SecondsTic_t
{
    // Seconds since beginning
    uint32_t second;
} JumpData_SecondsTic;

#endif //ifndef AUDIBLEA_JUMP_DATA_HH
