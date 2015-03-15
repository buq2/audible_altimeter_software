#include "ui_main_menu.hh"

MainMenu::MainMenu(Config *config, MiscInformation *misc)
    :
      config_(config),
      misc_(misc),
      display_(this),
      log_(this),
      alarms_(this),
      dz_(this),
      log_admin_(this),
      time_date_(this),
      config_apply_fun_(0)
{
}

Config *MainMenu::GetConfig()
{
    return config_;
}

MiscInformation *MainMenu::GetMiscInformation()
{
    return misc_;
}

void MainMenu::SetMiscInformation(MiscInformation *misc)
{
    misc_ = misc;
}

bool MainMenu::IsAtMainMenu() const
{
    return GetActiveItem() == this;
}

void MainMenu::SetConfigChangedFunction(MainMenu::ConfigChangedFunction fun)
{
    config_apply_fun_ = fun;
}

void MainMenu::KeyPress(const UiBase::KeyCode key, const bool down)
{
    UiMenu::KeyPress(key,down);

    // Config might have changed. Apply config changes
    if (NULL != config_apply_fun_) {
        config_apply_fun_(config_);
    }
}
