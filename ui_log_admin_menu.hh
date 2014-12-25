#ifndef UI_LOG_ADMIN_MENU
#define UI_LOG_ADMIN_MENU

#include "ui_menu.hh"

class UiLogAdminMenu
        :
        public UiMenu
{
 public:
    UiLogAdminMenu(UiMenu *parent);
    const char *GetLabel();
}; //class UiLogAdminMenu

#endif //ifndef UI_LOG_ADMIN_MENU
