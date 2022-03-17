#ifndef AUDIBLEA_JUMP_DATA_HH
#define AUDIBLEA_JUMP_DATA_HH

#include "config.hh"

// First sector (beginning from address 0) contains:
// 1)  configuration data (Config)

// Next sectors will contain jump log data

#define JUMPDATA_HEADER_MAGIC 0xBEEFCA5E

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

// Stored at the beginning of each 4kb sector
typedef PACK(struct JumpData_Header_t
{
    JumpData_Header_t()
        :
          magic_number(0xBEEFCA5E)
    {
    }
    uint32_t magic_number; // Magic number = 0xBEEFCA5E. If sector does not begin with this number -> invalid sector
    uint32_t jump_number; // Starts from 1.
}) JumpData_Header;

// After JumpData_Header begins the actual jump data. Before each struct
// we have uint8_t which describes next struct type:

typedef enum JumpData_StructEnum_t
{
    JUMPDATA_STRUCT_ENUM_JUMP_INFO = 1, //JumpData_Date_Time
    JUMPDATA_STRUCT_ENUM_BEGIN_SENSOR_DATA = 2, // JumpData_BeginSensorData
    JUMPDATA_STRUCT_ENUM_ALTITUDE = 3, // JumpData_Altitude
    JUMPDATA_STRUCT_ENUM_ALTITUDE_RATE = 4, // JumpData_AltitudeRate
    JUMPDATA_STRUCT_ENUM_SECONDS_TIC = 5, // JumpData_SecondsTic
    JUMPDATA_STRUCT_ENUM_DATA_PADDING = 6 // JumpData_Padding
} JumpData_StructEnum;

typedef PACK(struct JumpData_Padding_t
{
    uint32_t padding_bytes; // how many bytes should be skipped AFTER this struct
}) JumpData_Padding;

typedef PACK(struct JumpData_Info_t
{
    char jump_date_time[20]; //format "2015-03-11 20:23:10\0"
    char dropzone[32]; //freeformat
}) JumpData_Date_Info;

typedef PACK(struct JumpData_BeginSensorData_t
{
    // Time in seconds since last sensor data update
    float time_since_last_sensor_data_seconds; 
}) JumpData_BeginSensorData;

typedef PACK(struct JumpData_Altitude_t
{
    uint8_t altitude_sensor_id;
    float altitude_meters;
}) JumpData_Altitude;

typedef PACK(struct JumpData_AltitudeRate_t
{
    float altitude_rate_m_per_s;
}) JumpData_AltitudeRate;

typedef PACK(struct JumpData_SecondsTic_t
{
    // Seconds since beginning
    uint32_t second;
}) JumpData_SecondsTic;

#endif //ifndef AUDIBLEA_JUMP_DATA_HH
