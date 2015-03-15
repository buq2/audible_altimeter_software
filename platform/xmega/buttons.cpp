#include "buttons.hh"
#include <avr/interrupt.h>

using namespace axlib;



Buttons *global_buttons_obj = 0;

ISR(TCD0_OVF_vect)
{
    if (0 != global_buttons_obj) {
        global_buttons_obj->UpdateButtonCounters();
    }
}

void Buttons::StopCounter()
{
    TCD0.CTRLA = TC_CLKSEL_OFF_gc;
}

void Buttons::StartCounter()
{
    // First button press should be registered quickly
    // (counter oveflows faster from 0->1 than from 1->2)
    TCD0.CNT = 2500;
    TCD0.PER = 3000;
    TCD0.INTCTRLA |= TC_OVFINTLVL_LO_gc;
    TCD0.CTRLA = TC_CLKSEL_DIV1024_gc;
}

void Buttons::UpdateButtonCounters()
{

    const bool change1 = UpdateButtonState(port_up_, pin_up_, &state_up_, &up_press_counter_);
    const bool change2 = UpdateButtonState(port_center_, pin_center_, &state_center_, &center_press_counter_);
    const bool change3 = UpdateButtonState(port_down_, pin_down_, &state_down_, &down_press_counter_);

    if (state_changed_fun_ != 0 && ((change1 || change2 || change3) || !AnyButtonPressed())) {
        state_changed_fun_();
    }
}

Buttons::Buttons(const Port port_up, const Pin pin_up,
                 const Port port_center, const Pin pin_center,
                 const Port port_down, const Pin pin_down)
    :
      state_changed_fun_(0),
      port_up_(GetPort(port_up)),
      port_center_(GetPort(port_center)),
      port_down_(GetPort(port_down)),
      pin_up_((uint8_t)pin_up),
      pin_center_((uint8_t)pin_center),
      pin_down_((uint8_t)pin_down)
{
    global_buttons_obj = this;

    // Set button pins to inputs
    port_up_->DIRCLR = pin_up_;
    port_center_->DIRCLR = pin_center_;
    port_down_->DIRCLR = pin_down_;

    // Set internal pullups
    *(GetPortPinControlRegister(port_up,pin_up)) |= PORT_OPC_WIREDANDPULL_gc;
    *(GetPortPinControlRegister(port_center,pin_center)) |= PORT_OPC_WIREDANDPULL_gc;
    *(GetPortPinControlRegister(port_down,pin_down)) |= PORT_OPC_WIREDANDPULL_gc;
}

void Buttons::CheckState()
{
    // Called when button is pressed or released
    if (AnyButtonPressed()) {
        StartCounter();
    } else {
        StopCounter();
        UpdateButtonCounters();
    }
}

bool Buttons::AnyButtonPressed()
{
    return !(port_up_->IN & pin_up_) ||
            !(port_center_->IN & pin_center_) ||
            !(port_down_->IN & pin_down_);
}

void Buttons::SetButtonStateChangedFunction(Buttons::button_state_change_fun fun)
{
    state_changed_fun_ = fun;
}

bool Buttons::UpdateButtonState(PORT_t *port, uint8_t pin,
                                Buttons::ButtonState *state, uint8_t *counter)
{
    const uint8_t pressed_limit = 1;
    const uint8_t long_limit = 10;
    const ButtonState prev_state = *state;
    if ((port->IN & pin) == 0) {
        ++(*counter);
        if (*state == BUTTON_LONG) {
            *state = BUTTON_EXTRA_LONG;
        } else if (*state != BUTTON_EXTRA_LONG && *counter > long_limit) {
            *state = BUTTON_LONG;
        }
    } else {
        // Button not pressed
        if (*counter > pressed_limit && *counter < long_limit) {
            *state = BUTTON_SHORT;
        } else {
            *state = BUTTON_OFF;
        }
        *counter = 0;
    }

    return prev_state != *state;
}

