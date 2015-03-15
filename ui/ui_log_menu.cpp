#include "ui_log_menu.hh"


UiLogMenu::UiLogMenu(UiMenu *parent)
    :
      UiMenu(parent),
      save_mode_(this,"Save mode"),
      memory_usage_(this,"Mem usg", 0, 536870912)
{
    Config *conf = GetConfig();
    save_mode_.SetEnumPointer(&conf->log_save_mode);

    MiscInformation *misc = GetMiscInformation();
    memory_usage_.SetPointer(&misc->current_memory_usage);
}

const char *UiLogMenu::GetLabel()
{
    static const char label[] = "Log";
    return label;
}
