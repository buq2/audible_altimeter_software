#include "ui_main.hh"
#include <iostream>

UiMain::UiMain(Sensors *sensors)
    :
      altimeter_(sensors)
{

}

void UiMain::Render(DisplayBuffer *buffer)
{
    altimeter_.Render(buffer);
}

void UiMain::KeyPress(const UiBase::KeyCode key, const bool down)
{

}

void UiMain::Tick100ms()
{
    altimeter_.Tick100ms();
    menu_.Tick100ms();
}
