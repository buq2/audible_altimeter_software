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

// If we ever run pure virtual funciton, stop
extern "C" void __cxa_pure_virtual() { while (1); }

#define STOP_IF_ERROR(x) {if(x){while(1){}}}

bool global_usb_connected = false;
UiMain *global_ui_main;
DisplayBuffer *global_display_buffer;
Buttons global_buttons(PORT_C,PIN_4,
                PORT_C,PIN_3,
                PORT_C,PIN_2);
DisplaySharp *global_display;
SensorController *global_sensor_ctrl;
bool global_setup_ready = false;
Sensors *global_sensors;
ClockMcp7940M *global_clock;

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
ISR(PORTC_INT0_vect)
{
    global_buttons.CheckState();
}

void ButtonStateChangedCallback()
{
    // Update buttons
    if (global_buttons.GetDown() == Buttons::BUTTON_LONG || global_buttons.GetDown() == Buttons::BUTTON_SHORT) {
        global_ui_main->KeyPress(UiBase::KEY_DOWN, true);
    }
    if (global_buttons.GetUp() == Buttons::BUTTON_LONG || global_buttons.GetUp() == Buttons::BUTTON_SHORT) {
        global_ui_main->KeyPress(UiBase::KEY_UP, true);
    }
    if (global_buttons.GetCenter() == Buttons::BUTTON_SHORT) {
        global_ui_main->KeyPress(UiBase::KEY_RIGHT, true);
    } else if (global_buttons.GetCenter() == Buttons::BUTTON_LONG) {
        global_ui_main->KeyPress(UiBase::KEY_LEFT, true);
    }
    if (global_buttons.GetCenter() == Buttons::BUTTON_OFF &&
        global_buttons.GetUp() == Buttons::BUTTON_EXTRA_LONG &&
        global_buttons.GetDown() == Buttons::BUTTON_EXTRA_LONG)
    {
        // Zero altitude
        global_sensor_ctrl->ZeroAltitude();
    }
}

void SetupButtonInterrupts()
{
    // Interrupts on both edges
    PORTC.PIN2CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
    PORTC.PIN3CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
    PORTC.PIN4CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;

    PORTC.INT0MASK |= (uint8_t)PIN_2;
    PORTC.INT0MASK |= (uint8_t)PIN_3;
    PORTC.INT0MASK |= (uint8_t)PIN_4;

    PORTC.INTCTRL |= PORT_INT0LVL_LO_gc;

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

void UpdateUsb()
{
    if (global_usb_connected) {
        // Usb related
        CDC_Device_SendString(&VirtualSerial_CDC_Interface, global_sensors->GetAltitudeMetersString());
        CDC_Device_SendString(&VirtualSerial_CDC_Interface, global_clock->GetTimeString());
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);

//        {
//            char str[6];
//            sprintf(str,"%d\n\r",rtc_counter);
//            CDC_Device_SendString(&VirtualSerial_CDC_Interface, str);
//        }
    }
    USB_USBTask();
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

void UpdateConfig(Config *conf)
{
    global_display_buffer->SetRotation(conf->display_orientation);
    global_ui_main->GetAltimeterUi()->SetUiMode(conf->default_altimeter_ui_mode_);
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
                         PORT_A, 0b00001000, //cs
                         PORT_NOT_USED, 0, //excomin
                         PORT_A, 0b00000100 //display on
                         );
    display.SetDisplayOn(true);
    global_display = &display;
    Sensors sensors;
    global_sensors = &sensors;

    UiMain ui(&config, &sensors,UpdateConfig);
    global_ui_main = &ui;

    ClockMcp7940M clock(PORT_C);
    global_clock = &clock;
    clock.Setup();
    clock.Setup1HzSquareWave();

    FlashS25Fl216K flash(PORT_C, PORT_A, PIN_4);
    MemoryController mem_control(&flash);

    SensorController sensor_ctrl(&sensors, &mem_control, &clock);
    sensor_ctrl.Setup();
    global_sensor_ctrl = &sensor_ctrl;

    display.Setup();
    display.Clear();

    SetupHardware();
    GlobalInterruptEnable();

    while (1) {

    }
}
