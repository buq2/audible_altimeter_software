#include "ui_dz_menu.hh"


UiDzMenu::UiDzMenu(UiMenu *parent)
    :
      UiMenu(parent)
{

}

const char *UiDzMenu::GetLabel()
{
    static const char label[] = "DZ Setup";
    return label;
}
