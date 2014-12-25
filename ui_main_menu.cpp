#include "ui_main_menu.hh"

MainMenu::MainMenu()
{
    UiMenuItem *m1 = new UiMenuItem(this, "item1");
    UiMenuItem *m2 = new UiMenuItem(this, "item2");
    UiMenuItem *m3 = new UiMenuItem(this, "item3");
    UiMenuItem *m4 = new UiMenuItem(this, "item4");
    UiMenuItem *m5 = new UiMenuItem(this, "item5");
    UiMenuItem *m6 = new UiMenuItem(this, "item6");
    UiMenuItem *m7 = new UiMenuItem(this, "item7");
    UiMenuItem *m8 = new UiMenuItem(this, "item8");
    UiMenuItem *m9 = new UiMenuItem(this, "item9");
    UiMenuItem *m10 = new UiMenuItem(this, "item10");
    UiMenuItem *m11 = new UiMenuItem(this, "item11");
    UiMenuItem *m12 = new UiMenuItem(this, "item12");
    UiMenuItem *m13 = new UiMenuItem(this, "item13");
    UiMenuItem *m14 = new UiMenuItem(this, "item14");
    UiMenuItem *m15 = new UiMenuItem(this, "item15");
    UiMenuItem *m16 = new UiMenuItem(this, "item16");

    new UiMenuItem(m1, "item1-sub1");
    new UiMenuItem(m1, "item1-sub2");
    new UiMenuItem(m1, "item1-sub3");

    new UiMenuItem(m2, "item2-abc");
    new UiMenuItem(m2, "item2-cde");
    new UiMenuItem(m2, "item2-efg");

    new UiMenuItem(m3, "item3-a");
    new UiMenuItem(m3, "item3-b");

    new UiMenuItem(m4, "item4-a");
}

