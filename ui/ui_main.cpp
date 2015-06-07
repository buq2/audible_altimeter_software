#include "ui_main.hh"

UiMain::UiMain(Config *config, Sensors *sensors, MiscInformation *misc, AltitudeManager *alt_manager)
    :
      altimeter_(sensors,alt_manager),
      menu_(config, misc),
      ui_selection_(UI_SELECTION_ALTIMETER),
      config_save_(0)
{

}

void UiMain::Render(DisplayBuffer *buffer)
{
    switch(ui_selection_) {
    case UI_SELECTION_MENU:
        menu_.Render(buffer);
        break;
    case UI_SELECTION_ALTIMETER:
    default:
        altimeter_.Render(buffer);
        break;
    }
}

void UiMain::KeyPress(const UiBase::KeyCode key, const bool down)
{    
    switch(ui_selection_) {
    case UI_SELECTION_MENU:
        if (down && key == KEY_LEFT && menu_.IsAtMainMenu()) {
            ui_selection_ = UI_SELECTION_ALTIMETER;

            // Back at altimeter
            // Save configs
            if (config_save_ != 0) {
                config_save_(menu_.GetConfig());
            }

            return;
        }
        menu_.KeyPress(key,down);
        break;
    case UI_SELECTION_ALTIMETER:
    default:
        if (down && key == KEY_LEFT) {
            ui_selection_ = UI_SELECTION_MENU;
            return;
        }
        altimeter_.KeyPress(key,down);
        break;
    }
}

void UiMain::Tick100ms()
{
    altimeter_.Tick100ms();
    menu_.Tick100ms();
}

UiAltimeter *UiMain::GetAltimeterUi()
{
    return &altimeter_;
}

void UiMain::SetConfigChangedFunction(MainMenu::ConfigChangedFunction fun)
{
    menu_.SetConfigChangedFunction(fun);
}

void UiMain::SetConfigSaveFunction(UiMain::ConfigSaveFunction fun)
{
    config_save_ = fun;
}


