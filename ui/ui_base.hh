#ifndef UI_BASE_HH
#define UI_BASE_HH

#include "axlib/displays/display_buffer.hh"

class UiBase
{
 public:
    UiBase();

    typedef enum KeyCode_t
    {
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_MIDDLE
    } KeyCode;

    virtual void Render(axlib::DisplayBuffer *buffer) = 0;
    virtual void KeyPress(const KeyCode key, const bool down) = 0;
    virtual void Tick100ms();
}; //class Ui

#endif //ifndef UI_BASE_HH
