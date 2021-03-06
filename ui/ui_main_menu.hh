#ifndef UI_MAIN_MENU_HH
#define UI_MAIN_MENU_HH

#include "ui_menu.hh"
#include "ui_display_menu.hh"
#include "ui_log_menu.hh"
#include "ui_alarms_menu.hh"
#include "ui_dz_menu.hh"
#include "ui_log_admin_menu.hh"
#include "ui_time_date_menu.hh"
#include "ui_advanced_menu.hh"

class MainMenu
        :
        public UiMenu
{
 public:
    typedef void(*ConfigChangedFunction)(Config*);
    MainMenu(Config *config, MiscInformation *misc);
    virtual Config *GetConfig();
    virtual MiscInformation *GetMiscInformation();
    virtual void SetMiscInformation(MiscInformation *misc);
    bool IsAtMainMenu() const;
    void SetConfigChangedFunction(ConfigChangedFunction fun);
    virtual void KeyPress(const UiBase::KeyCode key, const bool down);
 private:
    Config *config_;
    MiscInformation *misc_;
    UiDisplayMenu display_;
    UiLogMenu log_;
    UiAlarmsMenu alarms_;
    UiDzMenu dz_;
    UiLogAdminMenu log_admin_;
    UiTimeDateMenu time_date_;
    UiAdvancedMenu advanced_;
    ConfigChangedFunction config_apply_fun_;
}; //class MainMenu

#endif //ifndef UI_MAIN_MENU_HH
