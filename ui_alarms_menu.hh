#ifndef UI_ALARMS_MENU
#define UI_ALARMS_MENU

#include "ui_menu.hh"

class UiAlarmsMenu
        :
        public UiMenu
{
 public:
    UiAlarmsMenu(UiMenu *parent);
    const char *GetLabel();
}; //class UiAlarmsMenu

#endif //ifndef UI_ALARMS_MENU
