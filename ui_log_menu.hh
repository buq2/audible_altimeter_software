#ifndef UI_LOG_MENU
#define UI_LOG_MENU

#include "ui_menu.hh"

class UiLogMenu
        :
        public UiMenu
{
 public:
    UiLogMenu(UiMenu *parent);
    const char *GetLabel();
}; //class UiLogMenu

#endif //ifndef UI_LOG_MENU
