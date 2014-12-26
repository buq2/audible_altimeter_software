#ifndef UI_TIME_DATE_MANU_HH
#define UI_TIME_DATE_MANU_HH

#include "ui_menu.hh"

class UiTimeDateMenu
        :
        public UiMenu
{
 public:
    UiTimeDateMenu(UiMenu *parent);
    const char *GetLabel();
}; //class UiTimeDateMenu

#endif //ifndef UI_TIME_DATE_MANU_HH
