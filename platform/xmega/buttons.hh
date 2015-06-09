#ifndef BUTTONS_HH
#define BUTTONS_HH

#include "axlib/core/io.hh"

class Buttons
{
 public:
    Buttons(const axlib::Port port_up, const axlib::Pin pin_up,
            const axlib::Port port_center, const axlib::Pin pin_center,
            const axlib::Port port_down, const axlib::Pin pin_down);

    void CheckState();

    typedef enum
    {
        BUTTON_OFF,
        BUTTON_SHORT,
        BUTTON_LONG,
        BUTTON_EXTRA_LONG
    } ButtonState;

    /// \return State of Up-button
    ButtonState GetUp();

    /// \return State of Center-button
    ButtonState GetCenter();

    /// \return State of Down-button
    ButtonState GetDown();

    /// \return How long up button has been pressed
    uint8_t GetCounterUp();

    /// \return How long center button has been pressed
    uint8_t GetCounterCenter();

    /// \return How long down button has been pressed
    uint8_t GetCounterDown();

    /// \return True if any button is being pressed (any amount of time)
    bool AnyButtonPressed();

    /// Update button counters. This function should be called periodically
    /// in relatively quick fashion (10 times a second or faster)
    void UpdateButtonCounters();

    typedef void(*button_state_change_fun)(void);
    /// Set function which will be called if button state changes
    /// \param[in] fun Function which will be called if change happens
    void SetButtonStateChangedFunction(button_state_change_fun fun);
 private:
    /// Update button state
    /// \param[in] port Location of the button pin
    /// \param[in] pin Pin number of the button
    /// \param[in,out] state State of the button which should be updated
    /// \param[out] counter Counter which counts how long button has been pressed
    bool UpdateButtonState(PORT_t *port, uint8_t pin,
                           ButtonState *state,
                           uint8_t *counter);
    void StopCounter();
    void StartCounter();
 private:
    // Function which is called if state of any of the buttons change
    button_state_change_fun state_changed_fun_;

    // Ports and pins used by the buttons
    PORT_t *port_up_;
    PORT_t *port_center_;
    PORT_t *port_down_;
    uint8_t pin_up_;
    uint8_t pin_center_;
    uint8_t pin_down_;

    // Counter which tell how long a button has been pressed
    uint8_t up_press_counter_;
    uint8_t center_press_counter_;
    uint8_t down_press_counter_;

    // States of the buttons
    ButtonState state_up_;
    ButtonState state_center_;
    ButtonState state_down_;
}; //class Buttons

#endif //ifndef BUTTONS_HH
