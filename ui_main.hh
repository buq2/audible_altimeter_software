#ifndef UI_MAIN_HH
#define UI_MAIN_HH

#include "ui_base.hh"

class UiMain
        :
        public UiBase
{
 public:
    UiMain();
    void Render(DisplayBuffer *buffer);
    void KeyPress(const UiBase::KeyCode key, const bool down);
 private:
}; //class UiMain

#endif //ifndef UI_MAIN_HH
