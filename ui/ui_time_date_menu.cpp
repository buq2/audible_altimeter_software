#include "ui_time_date_menu.hh"


UiTimeDateMenu::UiTimeDateMenu(UiMenu *parent)
    :
      UiMenu(parent)
{

}

const char *UiTimeDateMenu::GetLabel()
{
    static const char label[] = "Time and date";
    return label;
}
