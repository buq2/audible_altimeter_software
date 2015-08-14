#include "setup.hh"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <LUFA/Platform/Platform.h>
#include <LUFA/Drivers/Peripheral/TWI.h>
#include <LUFA/Drivers/Peripheral/SPI.h>
#include <LUFA/Drivers/USB/USB.h>
#include "axlib/core/io.hh"
#include "components.hh"

void SetupClockInterrupts()
{
    PORTB.PIN2CTRL |= PORT_OPC_PULLUP_gc | PORT_ISC_RISING_gc;
    PORTB.INT0MASK |= (uint8_t)PIN_2; //Enabled interrupt0 for pin2
    PORTB.INTCTRL |= PORT_INT0LVL_LO_gc;
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
    GlobalInterruptEnable();
}
