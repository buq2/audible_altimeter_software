#ifndef UI_MAIN_HH
#define UI_MAIN_HH

#include "ui_base.hh"
#include "ui_altimeter.hh"
#include "ui_menu.hh"
#include "sensors.hh"

class UiMain
        :
        public UiBase
{
 public:
    UiMain(Sensors *sensors);
    void Render(DisplayBuffer *buffer);
    void KeyPress(const UiBase::KeyCode key, const bool down);
    void Tick100ms();
 private:
    UiAltimeter altimeter_;
    UiMenu menu_;
}; //class UiMain

#endif //ifndef UI_MAIN_HH
