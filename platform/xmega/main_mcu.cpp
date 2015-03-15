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
#include "timer.hh"
#include "axlib/memory/flash_s25fl216k.hh"
#include "memory.hh"
#include "sensor_controller.hh"

// If we ever run pure virtual funciton, stop
extern "C" void __cxa_pure_virtual() { while (1); }

#define STOP_IF_ERROR(x) {if(x){while(1){}}}

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs.
 */
static FILE USBSerialStream;

bool global_usb_connected = false;
UiMain *global_ui_main;
DisplayBuffer *global_display_buffer;
Timer global_timer;
Buttons global_buttons(PORT_C,PIN_4,
                PORT_C,PIN_3,
                PORT_C,PIN_2);
SensorController *global_sensor_ctrl;

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

ISR(PORTB_INT0_vect)
{
    // 1HZ interrupt from the clock
    global_timer.TickApproxOneSecond();
}

void SetupClockInterrupts()
{
    PORTB.PIN2CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_RISING_gc;
    PORTB.INT0MASK |= (uint8_t)PIN_2; //Enabled interrupt0 for pin2
    PORTB.INTCTRL |= PORT_INT0LVL_LO_gc;
}

uint8_t button_counter = 0;
ISR(PORTC_INT0_vect)
{
    // Button press interrupt
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

/** Configures the board hardware and chip peripherals for the demo's functionality. */
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
}

void UpdateConfig(Config *conf)
{
    global_display_buffer->SetRotation(conf->display_orientation);
    global_ui_main->GetAltimeterUi()->SetUiMode(conf->default_altimeter_ui_mode_);
}

int main()
{

    SetupHardware();

    /* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
    CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);
    GlobalInterruptEnable();

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
    Sensors sensors;

    ClockMcp7940M clock(PORT_C);
    clock.Setup();
    clock.Setup1HzSquareWave();

    FlashS25Fl216K flash(PORT_C, PORT_A, PIN_4);
    MemoryController mem_control(&flash);

    SensorController sensor_ctrl(&sensors, &mem_control, &clock);
    sensor_ctrl.Setup();
    global_sensor_ctrl = &sensor_ctrl;

    UiMain ui(&config, &sensors,UpdateConfig);
    global_ui_main = &ui;

    display.Setup();
    display.Clear();

    while (1) {
        // Update altimeters
        sensor_ctrl.RequestDataUpdate();

        {
            // Display related
            display.ToggleExtcomin();
            ui.Tick100ms();
            ui.Render(&buffer);
            display.SetContent(buffer);
        }

        if (global_usb_connected) {
            // Usb related
            CDC_Device_SendString(&VirtualSerial_CDC_Interface, sensors.GetAltitudeMetersString());
            CDC_Device_SendString(&VirtualSerial_CDC_Interface, clock.GetTimeString());
            CDC_Device_USBTask(&VirtualSerial_CDC_Interface);

            {
                char str[6];

                sprintf(str,"%d\n\r",global_buttons.GetCounterUp());
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, str);
                sprintf(str,"%d\n\r",global_buttons.GetCounterCenter());
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, str);
                sprintf(str,"%d\n\r",global_buttons.GetCounterDown());
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, str);

                sprintf(str,"%d\n\r",global_buttons.GetUp());
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, str);
                sprintf(str,"%d\n\r",global_buttons.GetCenter());
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, str);
                sprintf(str,"%d\n\r",global_buttons.GetDown());
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, str);
            }

            USB_USBTask();
        } else {
            USB_USBTask();
        }

        {
            const float time_since_update = global_timer.Toc();
            sensor_ctrl.DataUpdate(time_since_update);
            global_timer.Tic();
        }
    }
}
