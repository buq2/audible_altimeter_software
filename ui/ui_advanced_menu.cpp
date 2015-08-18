#include "ui_advanced_menu.hh"

UiAdvancedMenu::UiAdvancedMenu(UiMenu *parent)
    :
      UiMenu(parent),
      oversample_rate_(this,"Ovrsmpl rt")
{
    Config *conf = GetConfig();
    oversample_rate_.SetEnumPointer(&conf->oversample_rate);
}

const char *UiAdvancedMenu::GetLabel()
{
    static const char label[] = "Advanced";
    return label;
}

