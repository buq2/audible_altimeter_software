#include <avr/io.h>
#include "config.hh"
#include "display_buffer.hh"
#include "sensors.hh"
#include "ui/ui_main.hh"

// If we ever run pure virtual funciton, stop
extern "C" void __cxa_pure_virtual() { while (1); }

int main()
{
    Config config;
    DisplayBuffer buffer(128,128);
    Sensors sensors;
    UiMain ui(&config, &sensors);
    
    while (1) {
        ui.Tick100ms();
        ui.Render(&buffer);
    }
}
