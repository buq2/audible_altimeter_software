#ifndef UI_DZ_MENU
#define UI_DZ_MENU

#include "ui_menu.hh"

class UiDzMenu
        :
        public UiMenu
{
 public:
    UiDzMenu(UiMenu *parent);
    const char *GetLabel();
}; //class UiDzMenu

#endif //ifndef UI_DZ_MENU
