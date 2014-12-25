#include "ui_log_menu.hh"


UiLogMenu::UiLogMenu(UiMenu *parent)
    :
      UiMenu(parent)
{

}

const char *UiLogMenu::GetLabel()
{
    static const char label[] = "Log";
    return label;
}
