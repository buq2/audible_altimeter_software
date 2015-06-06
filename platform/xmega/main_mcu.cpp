#include <avr/io.h>
#include <avr/interrupt.h>
#include <LUFA/Platform/Platform.h>
#include <LUFA/Drivers/Peripheral/TWI.h>
#include <LUFA/Drivers/Peripheral/SPI.h>
#include <LUFA/Drivers/USB/USB.h>
#include "lufa_config/Descriptors.h"
#include "config.hh"
#include "axlib/displays/display_buffer.hh"
#include "sensors.hh"
#include "ui/ui_main.hh"
#include "buttons.hh"
#include "axlib/displays/display_sharp.hh"
#include "axlib/sensors/clock_mcp7940m.hh"
#include "axlib/memory/flash_s25fl216k.hh"
#include "memory.hh"
#include "sensor_controller.hh"
#include <stdlib.h>
#include "buzzer.hh"
#include "axlib/sensors/digipot_mcp4017t.hh"

// If we ever run pure virtual function, stop
extern "C" void __cxa_pure_virtual() { while (1); }

#define STOP_IF_ERROR(x) {if(x){while(1){}}}

bool global_usb_connected = false;
UiMain *global_ui_main;
DisplayBuffer *global_display_buffer;
Buttons global_buttons(PORT_D,PIN_2,
                PORT_D,PIN_1,
                PORT_D,PIN_0);
DigipotMcp4017T global_digipot(PORT_C);
DisplaySharp *global_display;
SensorController *global_sensor_ctrl;
MemoryController *global_mem_control;
Sensors *global_sensors;
ClockMcp7940M *global_clock;
MiscInformation global_misc_info;
Config *global_config;
FlashS25Fl216K *global_flash;
Buzzer *global_buzzer;

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

// 1HZ interrupt from the clock
ISR(PORTB_INT0_vect)
{
}

void SetupClockInterrupts()
{
    PORTB.PIN2CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_RISING_gc;
    PORTB.INT0MASK |= (uint8_t)PIN_2; //Enabled interrupt0 for pin2
    PORTB.INTCTRL |= PORT_INT0LVL_LO_gc;
}

// Button press interrupt
ISR(PORTD_INT0_vect)
{
    global_buttons.CheckState();
}

void ButtonStateChangedCallback()
{
    // Update buttons
    if (global_buttons.GetDown() == Buttons::BUTTON_LONG || global_buttons.GetDown() == Buttons::BUTTON_SHORT) {
        global_ui_main->KeyPress(UiBase::KEY_DOWN, true);
        global_buzzer->Beep();
    }
    if (global_buttons.GetUp() == Buttons::BUTTON_LONG || global_buttons.GetUp() == Buttons::BUTTON_SHORT) {
        global_ui_main->KeyPress(UiBase::KEY_UP, true);
        global_buzzer->Beep();
    }
    if (global_buttons.GetCenter() == Buttons::BUTTON_SHORT) {
        global_ui_main->KeyPress(UiBase::KEY_RIGHT, true);
        global_buzzer->Beep();
    } else if (global_buttons.GetCenter() == Buttons::BUTTON_LONG) {
        global_ui_main->KeyPress(UiBase::KEY_LEFT, true);
        global_buzzer->Beep();
    }
    if (global_buttons.GetCenter() == Buttons::BUTTON_OFF &&
        global_buttons.GetUp() == Buttons::BUTTON_EXTRA_LONG &&
        global_buttons.GetDown() == Buttons::BUTTON_EXTRA_LONG)
    {
        // Zero altitude
        global_sensor_ctrl->ZeroAltitude();
        global_buzzer->Beep(3);
    }
}

void SetupButtonInterrupts()
{
    // Interrupts on both edges
    PORTD.PIN0CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
    PORTD.PIN1CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
    PORTD.PIN2CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;

    PORTD.INT0MASK |= (uint8_t)PIN_0;
    PORTD.INT0MASK |= (uint8_t)PIN_1;
    PORTD.INT0MASK |= (uint8_t)PIN_2;

    PORTD.INTCTRL |= PORT_INT0LVL_LO_gc;

    global_buttons.SetButtonStateChangedFunction(ButtonStateChangedCallback);
}

void UpdateDisplayAndUi()
{
    // Display related
    global_display->ToggleExtcomin();
    global_ui_main->Tick100ms();
    global_ui_main->Render(global_display_buffer);
    global_display->SetContent(*global_display_buffer);
}

void UpdateSensors(float update_seconds)
{
    global_sensor_ctrl->DataUpdate(update_seconds);

    // Update altimeters
    global_sensor_ctrl->RequestDataUpdate();
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
    while(true) {
        uint32_t bytes = 64;
        global_mem_control->GetJumpData(jump_idx,(uint8_t*)temp_buffer,&bytes,&offset);

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
    const uint32_t num_jumps = global_mem_control->GetNumberOfJumps();
    for (uint32_t i = 0; i < num_jumps; ++i) {
        const uint32_t sector = global_mem_control->GetJumpIndexSector(i);
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
    if (c == '\r') {
        if (command == COMMAND_NONE) {
            if (0 == strcmp(input_buffer, "num_jumps")) {
                const uint32_t num_jumps = global_mem_control->GetNumberOfJumps();
                sprintf(temp_buffer, "%ld\n\r", num_jumps);
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, temp_buffer);
            } else if (0 == strcmp(input_buffer, "date")) {
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, global_clock->GetTimeString());
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
                global_sensor_ctrl->RunDemo(SensorController::DEMO_OFF);
            } else if (0 == strcmp(input_buffer, "demo_climb")) {
                global_sensor_ctrl->RunDemo(SensorController::DEMO_CLIMB);
            } else if (0 == strcmp(input_buffer, "demo_freefall")) {
                global_sensor_ctrl->RunDemo(SensorController::DEMO_FREEFALL);
            } else if (0 == strcmp(input_buffer, "demo_canopy")) {
                global_sensor_ctrl->RunDemo(SensorController::DEMO_CANOPY);
            } else if (0 == strcmp(input_buffer, "demo_ground")) {
                global_sensor_ctrl->RunDemo(SensorController::DEMO_GROUND);
            } else if (0 == strcmp(input_buffer, "altitude")) {
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, global_sensors->GetAltitudeMetersString());
            } else if (0 == strcmp(input_buffer, "erase")) {
                global_sensor_ctrl->QuickErase();
                global_config->log_save_mode = Config::DataSaveOff;
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, "Quick erase run.\n\rSaving disabled\n\r");
            } else if (0 == strcmp(input_buffer, "full_erase")) {
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, "Please wait. This can take about one minute\n\r");
                global_sensor_ctrl->FullErase();
                global_config->log_save_mode = Config::DataSaveOff;
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
                global_flash->ReadData((uint8_t*)temp_buffer, param1, size_temp_buffer);
                for (uint32_t i = 0; i < 16; ++i) {
                    char str[10];
                    sprintf(str,"%d\n\r",temp_buffer[i]);
                    CDC_Device_SendString(&VirtualSerial_CDC_Interface,str);
                    CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
                    USB_USBTask();
                }
                command = COMMAND_NONE; //Back to normal mode
            } else if (command == COMMAND_SET_USE_FAKE) {
                global_sensor_ctrl->SetUseFakeData(param1>0);
                command = COMMAND_NONE;
            } else if (command == COMMAND_SET_ALTITUDE) {
                const float p = atof(input_buffer);
                global_sensor_ctrl->SetFakeAltitude(p);
                command = COMMAND_NONE;
            } else if (command == COMMAND_SET_RATE) {
                const float p = atof(input_buffer);
                global_sensor_ctrl->SetFakeAltitudeChange(p);
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

void UpdateBuzzer()
{
    global_buzzer->Tick100ms();
}

// 100ms interrupt
ISR(RTC_OVF_vect)
{
    static uint8_t update_200ms = 1;
    update_200ms = !update_200ms;
    if (update_200ms) {
        // Update sensors once 200ms
        UpdateSensors(0.2);
    }
    UpdateDisplayAndUi();
    UpdateUsb();
    UpdateBuzzer();
}

void SetupRtc()
{
    // Setup RTC
    PR.PRGEN &= ~PR_RTC_bm; //?
    CLK.RTCCTRL = CLK_RTCSRC_ULP_gc | CLK_RTCEN_bm; //use internal 32khz ulp clock and enable it
    RTC.CTRL = 0;  // shut down rtc (needed as rtc has different clock domain)
    while (RTC.STATUS & RTC_SYNCBUSY_bm) {
        // essential to wait for this condition or the RTC doesn't work
    }
    RTC.INTCTRL |= RTC_OVFINTLVL_LO_gc;
    RTC.PER = 102; // ~100ms = 10fps (1.024khz/1)/10fps
    RTC.CNT = 0;
    RTC.CTRL = RTC_PRESCALER_DIV1_gc; //divide by
}

void SetupHardware(void)
{
    /* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
    XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
    XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

    /* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
    XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
    XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

    PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;

    /* Hardware Initialization */
    USB_Init();

    SetupClockInterrupts();
    SetupButtonInterrupts();
    SetupRtc();
}

void UpdateFromConfig(Config *conf)
{
    global_display_buffer->SetRotation(conf->display_orientation);
    global_ui_main->GetAltimeterUi()->SetUiMode(conf->default_altimeter_ui_mode_);
}

void UpdateFromConfigAfterSave(Config *conf)
{
    if (conf->log_save_mode == Config::DataSaveAll) {
        global_sensor_ctrl->StartSavingData();
    } else if (conf->log_save_mode == Config::DataSaveOff) {
        global_sensor_ctrl->StopSavingData();
    }
}

void SaveConfig(Config *conf)
{
    UpdateFromConfigAfterSave(conf);
    global_mem_control->WriteConfig(conf);
}

uint8_t LoadConfig(Config *conf)
{
    return global_mem_control->LoadConfig(conf);
}

int main()
{
    Config config;
    const uint8_t width = 128;
    const uint8_t height = 128;
    DisplayBuffer buffer(width,height);
    global_display_buffer = &buffer;
    DisplaySharp display(width,height,
                         PORT_C,  //spi
                         PORT_A, PIN_3, //cs
                         PORT_NOT_USED, 0, //excomin
                         PORT_A, PIN_2 //display on
                         );
    display.SetDisplayOn(true);
    global_display = &display;
    Sensors sensors;
    global_sensors = &sensors;

    UiMain ui(&config, &sensors, &global_misc_info);
    ui.SetConfigChangedFunction(UpdateFromConfig);
    ui.SetConfigSaveFunction(SaveConfig);
    global_ui_main = &ui;

    ClockMcp7940M clock(PORT_C);
    global_clock = &clock;
    clock.Setup();
    clock.Setup1HzSquareWave();

    FlashS25Fl216K flash(PORT_C, PORT_A, PIN_4);
    global_flash = &flash;
    MemoryController mem_control(&flash);
    global_mem_control = &mem_control;

    global_digipot.Setup();
    global_digipot.SetValue(255);

    SensorController sensor_ctrl(&sensors, &mem_control, &clock);
    sensor_ctrl.Setup();
    global_sensor_ctrl = &sensor_ctrl;
    global_sensor_ctrl->SetMiscInformation(&global_misc_info);

    display.Setup();
    display.Clear();

    global_config = &config;
    sensors.SetMiscInformation(&global_misc_info);

    Buzzer buzzer(PORT_D, PIN_4, &global_digipot);
    global_buzzer = &buzzer;

    SetupHardware();
    GlobalInterruptEnable();

    LoadConfig(&config);
    UpdateFromConfig(&config);
    UpdateFromConfigAfterSave(&config);

    while (1) {

    }
}
