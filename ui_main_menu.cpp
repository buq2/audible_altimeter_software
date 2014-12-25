#include "ui_main_menu.hh"

MainMenu::MainMenu(Config *config)
    :
      config_(config),
      display_(this),
      log_(this),
      alarms_(this),
      dz_(this),
      log_admin_(this),
      time_date_(this)
{
}

Config *MainMenu::GetConfig()
{
    return config_;
}
