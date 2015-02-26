#ifndef BUTTONS_HH
#define BUTTONS_HH

#include "axlib/core/io.hh"

class Buttons
{
 public:
    Buttons(const axlib::Port port_up, const axlib::Pin pin_up,
            const axlib::Port port_center, const axlib::Pin pin_center,
            const axlib::Port port_down, const axlib::Pin pin_down);
 private:
 private:
    PORT_t *port_up_;
    PORT_t *port_center_;
    PORT_t *port_down_;
    uint8_t pin_up_;
    uint8_t pin_center_;
    uint8_t pin_down_;
}; //class Buttons

#endif //ifndef BUTTONS_HH
