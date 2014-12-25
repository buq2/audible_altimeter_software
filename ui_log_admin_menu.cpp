#include "ui_log_admin_menu.hh"


UiLogAdminMenu::UiLogAdminMenu(UiMenu *parent)
    :
      UiMenu(parent)
{

}

const char *UiLogAdminMenu::GetLabel()
{
    static const char label[] = "Log Admin";
    return label;
}
