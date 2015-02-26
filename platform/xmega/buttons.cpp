#include "buttons.hh"

using namespace axlib;


Buttons::Buttons(const Port port_up, const Pin pin_up,
                 const Port port_center, const Pin pin_center,
                 const Port port_down, const Pin pin_down)
    :
      port_up_(GetPort(port_up)),
      port_center_(GetPort(port_center)),
      port_down_(GetPort(port_down)),
      pin_up_((uint8_t)pin_up),
      pin_center_((uint8_t)pin_center),
      pin_down_((uint8_t)pin_down)
{
    // Set button pins to inputs
    port_up_->DIRCLR = pin_up_;
    port_center_->DIRCLR = pin_center_;
    port_down_->DIRCLR = pin_down_;

    // Set internal pullups
    *(GetPortPinControlRegister(port_up,pin_up)) |= PORT_OPC_WIREDANDPULL_gc;
    *(GetPortPinControlRegister(port_center,pin_center)) |= PORT_OPC_WIREDANDPULL_gc;
    *(GetPortPinControlRegister(port_down,pin_down)) |= PORT_OPC_WIREDANDPULL_gc;
}
