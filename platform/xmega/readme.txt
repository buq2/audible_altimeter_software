Pins:

PORT_A
A0 -
A1 - 
A2 - DISP_ON
A3 - CSp_DISPLAY
A4 - CSn_FLASH
A5 -
A6 -
A7 -

PORT_B
B0 - 
B1 - ACC_INT
B2 - CLOCK_INT
B3 - BAROMETER_INT

PORT_C
C0 - I2C_SDA
C1 - I2C_SCL
C2 -
C3 -
C4 - 
C5 - SPI_MOSI
C6 - SPI_MISO
C7 - SPI_SCK

PORT_D
D0 - BUTTON_DOWN
D1 - BUTTON_CENTER
D2 - BUTTON_UP
D3 - 
D4 - BUZZER_DRIVE
D5 - 
D6 - USB_D-
D7 - USB_D+

PORT_D
E0 -
E1 -
E2 - RX
E3 - TX

Interrupts:
PORTB_INT0_vect - 1HZ interrupt from the clock (main_mcu.cpp)
TCD0_OVF_vect - Update button counters (buttons.cpp)
PORTD_INT0_vect - PORTD - INT0 - Button check state (main_mcu.cpp)
RTC_OVF_vect - RTC Overflow interrupt generates ~100ms signal (main_mcu.cpp)

Timer counters:
PORT_C TC0 -
PORT_C TC1 -
PORT_D TC0 - Button counter update
PORT_D TC1 - Buzzer signal
PORT_E TC0 - Buzzer modulation (beep start/stop, sweep step)
PORT_E TC1 -
