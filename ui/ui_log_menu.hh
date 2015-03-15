#ifndef UI_LOG_MENU
#define UI_LOG_MENU

#include "ui_menu.hh"
#include "ui_config_item_enum.hh"
#include "ui_config_item_int.hh"

class UiLogMenu
        :
        public UiMenu
{
 public:
    UiLogMenu(UiMenu *parent);
    const char *GetLabel();
 private:
    UiConfigItemEnum<Config::DataSaveMode> save_mode_;
    UiConfigItemInt<uint32_t> memory_usage_;
}; //class UiLogMenu

#endif //ifndef UI_LOG_MENU
