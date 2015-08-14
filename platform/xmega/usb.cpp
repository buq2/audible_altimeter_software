#include "usb.hh"
#include "lufa_config/Descriptors.h"
#include "components.hh"
#include <stdlib.h>

bool global_usb_connected = false;

void EVENT_USB_Device_Connect(void)
{
    global_usb_connected = true;
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    global_usb_connected = false;
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;

    ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

void PrintSerialHelp()
{
    CDC_Device_SendString(&VirtualSerial_CDC_Interface,
                          "Command list:\n\r"
                          "\n\r"
                          "help<CR>\n\r"
                          "num_jumps<CR>\n\r"
                          "jump_sectors<CR>\n\r"
                          "date<CR>\n\r"
                          "altitude<CR>\n\r"
                          "get_jump<CR>#<CR>\n\r"
                          "erase<CR>\n\r"
                          "erase_jumps<CR>\n\r"
                          "full_erase<CR>\n\r"
                          "print_data<CR>#<CR>\n\r"
                          "use_fake_data<CR>#<CR> (bool)\n\r"
                          "set_altitude<CR>#<CR> (fake altitude in [m]\n\r"
                          "set_rate<CR>#<CR> (fake altitude change rate in [m/s])\n\r"
                          "demo_off\n\r"
                          "demo_climb\n\r"
                          "demo_freefall\n\r"
                          "demo_canopy\n\r"
                          "demo_ground\n\r"
                          );
}

void SerialSendJumpIndex(const uint32_t jump_idx, char temp_buffer[64])
{
    uint32_t offset = 0;
    uint32_t bytes_send = 0;
    MemoryController *memctrl = GetComponents()->GetMemoryController();
    while(true) {
        uint32_t bytes = 64;
        memctrl->GetJumpData(jump_idx,(uint8_t*)temp_buffer,&bytes,&offset);

        if (bytes == 0) {
            // No more data to send
            break;;
        }
        bytes_send += bytes;
        CDC_Device_SendData(&VirtualSerial_CDC_Interface, temp_buffer, bytes);
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();
    }
}

void PrintJumpSectors(char temp_buffer[64])
{
    MemoryController *memctrl = GetComponents()->GetMemoryController();
    const uint32_t num_jumps = memctrl->GetNumberOfJumps();
    for (uint32_t i = 0; i < num_jumps; ++i) {
        const uint32_t sector = memctrl->GetJumpIndexSector(i);
        sprintf(temp_buffer,"%ld\n\r", sector);
        CDC_Device_SendString(&VirtualSerial_CDC_Interface, temp_buffer);
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();
    }
}

typedef enum Command_t
{
    COMMAND_NONE = 0,
    COMMAND_GET_JUMP = 1,
    COMMAND_PRINT_DATA = 2,
    COMMAND_SET_USE_FAKE = 3,
    COMMAND_SET_ALTITUDE = 4,
    COMMAND_SET_RATE = 5
} Command;

void HandleSerialInput()
{
    static char input_buffer[17];
    static uint8_t buffer_idx = 0;
    static Command command = COMMAND_NONE;
    input_buffer[buffer_idx] = 0;

    int16_t c = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
    if (c < 0) {
        // No data
        return;
    }

    const size_t size_temp_buffer = 64;
    char temp_buffer[size_temp_buffer];
    MemoryController *memctrl = GetComponents()->GetMemoryController();
    SensorController *sensorctrl = GetComponents()->GetSensorController();
    Sensors *sensors = GetComponents()->GetSensors();
    Config *config = GetComponents()->GetConfig();
    FlashS25Fl216K *flash = GetComponents()->GetFlash();

    if (c == '\r') {
        if (command == COMMAND_NONE) {
            if (0 == strcmp(input_buffer, "num_jumps")) {
                const uint32_t num_jumps = memctrl->GetNumberOfJumps();
                sprintf(temp_buffer, "%ld\n\r", num_jumps);
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, temp_buffer);
            } else if (0 == strcmp(input_buffer, "date")) {
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, GetComponents()->GetClock()->GetTimeString());
            } else if (0 == strcmp(input_buffer, "jump_sectors")) {
                PrintJumpSectors(temp_buffer);
            } else if (0 == strcmp(input_buffer, "get_jump")) {
                command = COMMAND_GET_JUMP;
            } else if (0 == strcmp(input_buffer, "print_data")) {
                command = COMMAND_PRINT_DATA;
            } else if (0 == strcmp(input_buffer, "use_fake_data")) {
                command = COMMAND_SET_USE_FAKE;
            } else if (0 == strcmp(input_buffer, "set_altitude")) {
                command = COMMAND_SET_ALTITUDE;
            } else if (0 == strcmp(input_buffer, "set_rate")) {
                command = COMMAND_SET_RATE;
            } else if (0 == strcmp(input_buffer, "demo_off")) {
                sensorctrl->RunDemo(SensorController::DEMO_OFF);
            } else if (0 == strcmp(input_buffer, "demo_climb")) {
                sensorctrl->RunDemo(SensorController::DEMO_CLIMB);
            } else if (0 == strcmp(input_buffer, "demo_freefall")) {
                sensorctrl->RunDemo(SensorController::DEMO_FREEFALL);
            } else if (0 == strcmp(input_buffer, "demo_canopy")) {
                sensorctrl->RunDemo(SensorController::DEMO_CANOPY);
            } else if (0 == strcmp(input_buffer, "demo_ground")) {
                sensorctrl->RunDemo(SensorController::DEMO_GROUND);
            } else if (0 == strcmp(input_buffer, "altitude")) {
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, sensors->GetAltitudeMetersString());
            } else if (0 == strcmp(input_buffer, "erase")) {
                sensorctrl->QuickErase();
                config->log_save_mode = Config::DataSaveOff;
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, "Quick erase run.\n\rSaving disabled\n\r");
            } else if (0 == strcmp(input_buffer, "erase_jumps")) {
                sensorctrl->QuickEraseJumps();
                config->log_save_mode = Config::DataSaveOff;
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, "Quick erase jumps run.\n\rSaving disabled\n\r");
            } else if (0 == strcmp(input_buffer, "full_erase")) {
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, "Please wait. This can take about one minute\n\r");
                sensorctrl->FullErase();
                config->log_save_mode = Config::DataSaveOff;
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, "Full erase run.\n\rSaving disabled\n\r");
            } else if (0 == strcmp(input_buffer, "help")) {
                PrintSerialHelp();
            } else {
                PrintSerialHelp();
            }
        } else {
            const uint32_t param1 = atoi(input_buffer);
            if (command == COMMAND_GET_JUMP) {
                SerialSendJumpIndex(param1, temp_buffer);
                command = COMMAND_NONE; //Back to normal mode
            } else if (command == COMMAND_PRINT_DATA) {
                flash->ReadData((uint8_t*)temp_buffer, param1, size_temp_buffer);
                for (uint32_t i = 0; i < 16; ++i) {
                    char str[10];
                    sprintf(str,"%d\n\r",temp_buffer[i]);
                    CDC_Device_SendString(&VirtualSerial_CDC_Interface,str);
                    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                    USB_USBTask();
                }
                command = COMMAND_NONE; //Back to normal mode
            } else if (command == COMMAND_SET_USE_FAKE) {
                sensorctrl->SetUseFakeData(param1>0);
                command = COMMAND_NONE;
            } else if (command == COMMAND_SET_ALTITUDE) {
                const float p = atof(input_buffer);
                sensorctrl->SetFakeAltitude(p);
                command = COMMAND_NONE;
            } else if (command == COMMAND_SET_RATE) {
                const float p = atof(input_buffer);
                sensorctrl->SetFakeAltitudeChange(p);
                command = COMMAND_NONE;
            }
        }
        buffer_idx = 0;
    } else {
        input_buffer[buffer_idx] = c;
        ++buffer_idx;
        buffer_idx %= 16;
    }
}

void UpdateUsb()
{
    if (global_usb_connected) {
        // Usb related
        HandleSerialInput();
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
    }
    USB_USBTask();
}
