#include "ui_alarms_menu.hh"


UiAlarmsMenu::UiAlarmsMenu(UiMenu *parent)
    :
      UiMenu(parent)
{

}

const char *UiAlarmsMenu::GetLabel()
{
    static const char label[] = "Alarms";
    return label;
}

