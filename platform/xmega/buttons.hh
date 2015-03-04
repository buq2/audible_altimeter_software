#ifndef BUTTONS_HH
#define BUTTONS_HH

#include "axlib/core/io.hh"

class Buttons
{
 public:
    Buttons(const axlib::Port port_up, const axlib::Pin pin_up,
            const axlib::Port port_center, const axlib::Pin pin_center,
            const axlib::Port port_down, const axlib::Pin pin_down);

    void Tick();

    typedef enum
    {
        BUTTON_OFF,
        BUTTON_SHORT,
        BUTTON_LONG,
        BUTTON_EXTRA_LONG
    } ButtonState;

    ButtonState GetUp()
    {
        return state_up_;
    }

    ButtonState GetCenter()
    {
        return state_center_;
    }

    ButtonState GetDown()
    {
        return state_down_;
    }

    uint8_t GetCounterUp()
    {
        return up_press_counter_;
    }
    uint8_t GetCounterCenter()
    {
        return center_press_counter_;
    }
    uint8_t GetCounterDown()
    {
        return down_press_counter_;
    }
 private:
    void UpdateButtonState(PORT_t *port, uint8_t pin,
                           ButtonState *state,
                           uint8_t *counter);
 private:
    PORT_t *port_up_;
    PORT_t *port_center_;
    PORT_t *port_down_;
    uint8_t pin_up_;
    uint8_t pin_center_;
    uint8_t pin_down_;

    uint8_t up_press_counter_;
    uint8_t center_press_counter_;
    uint8_t down_press_counter_;

    ButtonState state_up_;
    ButtonState state_center_;
    ButtonState state_down_;
}; //class Buttons

#endif //ifndef BUTTONS_HH
