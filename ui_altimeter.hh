#ifndef UI_ALTIMETER_HH
#define UI_ALTIMETER_HH

#include "ui_base.hh"

class UiAltimeter
        :
        public UiBase
{
 public:
    UiAltimeter();
    void Render(DisplayBuffer *buffer);
    void KeyPress(const UiBase::KeyCode key, const bool down);
 private:
};

#endif //ifndef UI_ALTIMETER_HH
