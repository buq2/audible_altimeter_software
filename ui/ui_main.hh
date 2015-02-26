#ifndef UI_MAIN_HH
#define UI_MAIN_HH

#include "ui_base.hh"
#include "ui_altimeter.hh"
#include "ui_main_menu.hh"
#include "sensors.hh"

class UiMain
        :
        public UiBase
{
 public:
    UiMain(Config *config, Sensors *sensors, MainMenu::ConfigApplyFunction fun = NULL);
    void Render(DisplayBuffer *buffer);
    void KeyPress(const UiBase::KeyCode key, const bool down);
    void Tick100ms();
    UiAltimeter *GetAltimeterUi();
    typedef enum UiSelection_t
    {
        UI_SELECTION_ALTIMETER,
        UI_SELECTION_MENU
    } UiSelection;
 private:
    UiAltimeter altimeter_;
    MainMenu menu_;
    UiSelection ui_selection_;
}; //class UiMain

#endif //ifndef UI_MAIN_HH
