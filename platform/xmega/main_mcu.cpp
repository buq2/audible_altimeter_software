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
#include "axlib/displays/display_sharp.hh"
#include "axlib/sensors/altimeter_mpl3115a2.hh"
#include "axlib/sensors/altimeter_ms5805_02ba01.hh"
#include "axlib/sensors/gps_sim33ela.hh"

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
    const uint8_t width = 128;
    const uint8_t height = 128;
    DisplayBuffer buffer(width,height);
    DisplaySharp display(width,height,
                         PORT_C,  //spi
                         PORT_A, 0b00001000, //cs
                         PORT_NOT_USED, 0, //excomin
                         PORT_A, 0b00000100 //display on
                         );
    display.SetDisplayOn(true);
    Sensors sensors;
    AltimeterMPl3114A2 alt1(PORT_C);
    AltimeterMS5805_02BA01 alt2(PORT_C);

    UiMain ui(&config, &sensors);

    display.Setup();
    display.Clear();
    const bool as_altimeter = true;
    alt1.SetMode(as_altimeter);
    alt1.SetOversampleRate(AltimeterMPl3114A2::OversampleRate128);
    STOP_IF_ERROR(alt1.Setup());
    STOP_IF_ERROR(alt2.Setup());

    while (1) {
        alt1.RequestDataUpdate();
        _delay_ms(100);
        display.ToggleExtcomin();
        float altitude_m = 0;
        if (0 == alt1.GetAltitudeMeters(&altitude_m)) {
            sensors.SetAltitudeMeters(altitude_m);
        }
        ui.Tick100ms();
        ui.Render(&buffer);
        display.SetContent(buffer);
        CDC_Device_SendString(&VirtualSerial_CDC_Interface, alt1.GetAltitudeMetersString());
        CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
        USB_USBTask();
    }
}
