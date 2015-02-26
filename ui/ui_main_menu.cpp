#include "ui_main_menu.hh"

MainMenu::MainMenu(Config *config, MainMenu::ConfigApplyFunction fun)
    :
      config_(config),
      display_(this),
      log_(this),
      alarms_(this),
      dz_(this),
      log_admin_(this),
      time_date_(this),
      config_apply_fun_(fun)
{
}

Config *MainMenu::GetConfig()
{
    return config_;
}

bool MainMenu::IsAtMainMenu() const
{
    return GetActiveItem() == this;
}

void MainMenu::KeyPress(const UiBase::KeyCode key, const bool down)
{
    UiMenu::KeyPress(key,down);

    // Config might have changed. Apply config changes
    if (NULL != config_apply_fun_) {
        config_apply_fun_(config_);
    }
}
