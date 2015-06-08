#ifndef UI_MAIN_HH
#define UI_MAIN_HH

#include "ui_base.hh"
#include "ui_altimeter.hh"
#include "ui_main_menu.hh"
#include "sensors.hh"
#include "altitude_manager.hh"

class UiMain
        :
        public UiBase
{
 public:
    UiMain(Config *config, Sensors *sensors, MiscInformation *misc,
           AltitudeManager *alt_manager);
    void Render(DisplayBuffer *buffer);
    void KeyPress(const UiBase::KeyCode key, const bool down);
    void Tick100ms();
    UiAltimeter *GetAltimeterUi();

    void SetConfigChangedFunction(MainMenu::ConfigChangedFunction fun);

    typedef void(*ConfigSaveFunction)(Config*);
    void SetConfigSaveFunction(ConfigSaveFunction fun);

    typedef enum UiSelection_t
    {
        UI_SELECTION_ALTIMETER,
        UI_SELECTION_MENU
    } UiSelection;
 private:
    UiAltimeter altimeter_;
    MainMenu menu_;
    UiSelection ui_selection_;
    ConfigSaveFunction config_save_;
}; //class UiMain

#endif //ifndef UI_MAIN_HH
