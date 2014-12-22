#include "ui_menu.hh"
#include <iostream>

const uint8_t menu_font_scale_x = 1;
const uint8_t menu_font_scale_y = 1;

UiMenu::UiMenu()
    :
      menu_font_(&FontStyle_vcr_tiny)
{
    MenuItem *m1 = new MenuItem(&root, "item1");
    MenuItem *m2 = new MenuItem(&root, "item2");
    MenuItem *m3 = new MenuItem(&root, "item3");
    MenuItem *m4 = new MenuItem(&root, "item4");
    MenuItem *m5 = new MenuItem(&root, "item5");
    MenuItem *m6 = new MenuItem(&root, "item6");
    MenuItem *m7 = new MenuItem(&root, "item7");
    MenuItem *m8 = new MenuItem(&root, "item8");
    MenuItem *m9 = new MenuItem(&root, "item9");
    MenuItem *m10 = new MenuItem(&root, "item10");
    MenuItem *m11 = new MenuItem(&root, "item11");
    MenuItem *m12 = new MenuItem(&root, "item12");
    MenuItem *m13 = new MenuItem(&root, "item13");
    MenuItem *m14 = new MenuItem(&root, "item14");
    MenuItem *m15 = new MenuItem(&root, "item15");
    MenuItem *m16 = new MenuItem(&root, "item16");

    new MenuItem(m1, "item1-sub1");
    new MenuItem(m1, "item1-sub2");
    new MenuItem(m1, "item1-sub3");

    new MenuItem(m2, "item2-abc");
    new MenuItem(m2, "item2-cde");
    new MenuItem(m2, "item2-efg");

    new MenuItem(m3, "item3-a");
    new MenuItem(m3, "item3-b");

    new MenuItem(m4, "item4-a");

    current_ = &root;
}

void UiMenu::Render(DisplayBuffer *buffer)
{
    buffer->Clear();

    const uint8_t max_rows = GetNumberOfMenuRows(*buffer);
    const uint8_t preferred_row = max_rows/2;
    const uint8_t row_height = GetMenuRowHeight();
    const uint8_t num = current_->GetNumberOfItems();
    const uint8_t selected = current_->GetSelectedItemIndex();

    // Calculate largest reasonable start position
    uint8_t start = selected < preferred_row ? 0 : selected - preferred_row;
    // Calculate end position
    const uint8_t end = std::min(start + max_rows - 1, num-1);
    // Fix start position according to end position
    start = end <= preferred_row*2 ? 0 : end - preferred_row*2;

    uint8_t ypos = 3;
    const char *select_str = "=";
    const uint8_t xpos_select = 3;
    const uint8_t xpos = DisplayBuffer::CalculateTextWidthPixels(*menu_font_, menu_font_scale_x, select_str)
            + xpos_select + 3;
    for (int i = start; i <= end; ++i) {

        if (i == current_->GetSelectedItemIndex()) {

            buffer->RenderText(*menu_font_, xpos_select, ypos,
                               menu_font_scale_x, menu_font_scale_y,
                               select_str);
        }

        MenuItem *item = current_->GetItem(i);

        buffer->RenderText(*menu_font_, xpos, ypos,
                           menu_font_scale_x, menu_font_scale_y,
                           item->GetLabel().c_str());
        ypos += row_height;
    }
}

void UiMenu::KeyPress(const UiBase::KeyCode key, const bool down)
{
    if (!down) {
        return;
    }

    switch(key) {
    case UiBase::KEY_UP:
        current_->Previous();
        break;
    case UiBase::KEY_DOWN:
        current_->Next();
        break;
    case UiBase::KEY_LEFT:
    {
        MenuItem *parent = current_->GetParent();
        if (NULL != parent) {
            current_ = parent;
        }
        break;
    }
    case UiBase::KEY_RIGHT:
    {
        MenuItem *sub = current_->GetItem(current_->GetSelectedItemIndex());
        if (NULL != sub) {
            current_ = sub;
        }
        break;
    }
    }
}

void UiMenu::Tick100ms()
{

}

uint8_t UiMenu::GetNumberOfMenuRows(const DisplayBuffer &buffer)
{
    return buffer.GetHeight() / GetMenuRowHeight() - 1;
}

uint8_t UiMenu::GetMenuRowHeight()
{
    return DisplayBuffer::CalculateTextHeightPixels(*menu_font_, 1);
}
