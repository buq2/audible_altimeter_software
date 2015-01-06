#include <avr/io.h>
#include <avr/interrupt.h>
#include <LUFA/Platform/Platform.h>
#include <LUFA/Drivers/Peripheral/TWI.h>
#include <LUFA/Drivers/Peripheral/SPI.h>
#include <LUFA/Drivers/USB/USB.h>
#include "lufa_config/Descriptors.h"
#include "config.hh"
#include "display_buffer.hh"
#include "sensors.hh"
#include "ui/ui_main.hh"
#include "display_sharp.hh"
#include "altimeter_mpl3115a2.hh"

// If we ever run pure virtual funciton, stop
extern "C" void __cxa_pure_virtual() { while (1); }

#define STOP_IF_ERROR(x) {if(x){while(1){}}}

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs.
 */
static FILE USBSerialStream;

void EVENT_USB_Device_Connect(void)
{

}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{

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

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
#if (ARCH == ARCH_AVR8)
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);
#elif (ARCH == ARCH_XMEGA)
    /* Start the PLL to multiply the 2MHz RC oscillator to 32MHz and switch the CPU core to run from it */
    XMEGACLK_StartPLL(CLOCK_SRC_INT_RC2MHZ, 2000000, F_CPU);
    XMEGACLK_SetCPUClockSource(CLOCK_SRC_PLL);

    /* Start the 32MHz internal RC oscillator and start the DFLL to increase it to 48MHz using the USB SOF as a reference */
    XMEGACLK_StartInternalOscillator(CLOCK_SRC_INT_RC32MHZ);
    XMEGACLK_StartDFLL(CLOCK_SRC_INT_RC32MHZ, DFLL_REF_INT_USBSOF, F_USB);

    PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
#endif

    /* Hardware Initialization */
    USB_Init();
}

int main()
{
    SetupHardware();

    /* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
    CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);
    GlobalInterruptEnable();

    Config config;
    const uint8_t width = 96;
    const uint8_t height = 96;
    DisplayBuffer buffer(width,height);
    DisplaySharp display(width,height);
    Sensors sensors;
    AltimeterMPl3114A2 alt1;
    UiMain ui(&config, &sensors);

    display.Setup();
    display.Clear();
    const bool as_altimeter = true;
    alt1.SetMode(as_altimeter);
    alt1.SetOversampleRate(AltimeterMPl3114A2::OversampleRate128);
    STOP_IF_ERROR(alt1.Setup());

    bool sensors_zeroed = false;
    while (1) {
        alt1.RequestDataUpdate();
        display.ToggleExtcomin();
        ui.Tick100ms();
        ui.Render(&buffer);
        display.SetContent(buffer);

        {
            // Sensor updates
            float altitude_meters;
            uint8_t err = alt1.GetAltitudeMeters(&altitude_meters);
            if (!err) {
                sensors.SetAltitudeMeters(altitude_meters);
            }
        }

        if (!sensors_zeroed) {
            // We should first use all other gizmos before
            // zeroing the sensors first time
            sensors_zeroed = true;
            alt1.ZeroAltitudeLoop();
        }

        CDC_Device_SendString(&VirtualSerial_CDC_Interface, alt1.GetAltitudeMetersString());
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();
    }
}
