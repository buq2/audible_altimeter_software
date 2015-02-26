#ifndef UI_MAIN_MENU_HH
#define UI_MAIN_MENU_HH

#include "ui_menu.hh"
#include "ui_display_menu.hh"
#include "ui_log_menu.hh"
#include "ui_alarms_menu.hh"
#include "ui_dz_menu.hh"
#include "ui_log_admin_menu.hh"
#include "ui_time_date_menu.hh"

class MainMenu
        :
        public UiMenu
{
 public:
    typedef void(*ConfigApplyFunction)(Config*);
    MainMenu(Config *config, ConfigApplyFunction fun = NULL);
    virtual Config *GetConfig();
    bool IsAtMainMenu() const;
    void SetConfigApplyFunction(ConfigApplyFunction);
    virtual void KeyPress(const UiBase::KeyCode key, const bool down);
 private:
    Config *config_;
    UiDisplayMenu display_;
    UiLogMenu log_;
    UiAlarmsMenu alarms_;
    UiDzMenu dz_;
    UiLogAdminMenu log_admin_;
    UiTimeDateMenu time_date_;
    ConfigApplyFunction config_apply_fun_;
}; //class MainMenu

#endif //ifndef UI_MAIN_MENU_HH
