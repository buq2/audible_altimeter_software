#include "ui_menu.hh"
#include <iostream>

const uint8_t menu_font_scale_x = 1;
const uint8_t menu_font_scale_y = 1;

UiMenu::UiMenu()
    :
      menu_font_(&FontStyle_vcr_tiny),
      selected_sub_(0)
{
    current_ = this;
}

UiMenu::UiMenu(UiMenu *parent)
    :
      UiMenuItem(parent)
{

}

void UiMenu::Render(DisplayBuffer *buffer)
{
    buffer->Clear();

    const uint8_t max_rows = GetNumberOfMenuRows(*buffer);
    const uint8_t preferred_row = max_rows/2;
    const uint8_t row_height = GetMenuRowHeight();
    const uint8_t num = GetNumberOfItems();
    const uint8_t selected = GetSelectedItemIndex();

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

        if (i == selected) {

            buffer->RenderText(*menu_font_, xpos_select, ypos,
                               menu_font_scale_x, menu_font_scale_y,
                               select_str);
        }

        UiMenuItem *item = GetItem(i);
        if (NULL == item) {
            // Over indexed, can not render
            break;
        }

        // Left side
        buffer->RenderText(*menu_font_, xpos, ypos,
                           menu_font_scale_x, menu_font_scale_y,
                           item->GetLabel());

        // Right side
        buffer->RenderText_AlignRight(*menu_font_, buffer->GetWidth()-xpos_select, ypos,
                           menu_font_scale_x, menu_font_scale_y,
                           item->GetLabelValue());

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
        Previous();
        break;
    case UiBase::KEY_DOWN:
        Next();
        break;
    case UiBase::KEY_LEFT:
    {
        UiMenuItem *parent = current_->GetParent();
        if (NULL != parent) {
            current_ = parent;
        }
        break;
    }
    case UiBase::KEY_RIGHT:
    {
        UiMenuItem *sub = GetItem(GetSelectedItemIndex());
        if (NULL != sub) {
            current_ = sub;
        }
        break;
    }
    }
}

uint8_t UiMenu::GetNumberOfMenuRows(const DisplayBuffer &buffer)
{
    return buffer.GetHeight() / GetMenuRowHeight() - 1;
}

uint8_t UiMenu::GetMenuRowHeight()
{
    return DisplayBuffer::CalculateTextHeightPixels(*menu_font_, 1);
}

uint8_t UiMenu::GetSelectedItemIndex()
{
    return selected_sub_;
}

void UiMenu::Next()
{
    selected_sub_++;
    selected_sub_ = std::min(selected_sub_,(uint8_t)(GetNumberOfItems()-1));
}

void UiMenu::Previous()
{
    if (selected_sub_ == 0) {
        return;
    }
    --selected_sub_;
}

uint8_t UiMenu::GetNumberOfItems()
{
    return items_.size();
}

UiMenuItem *UiMenu::GetItem(const uint8_t i)
{
    if (i >= items_.size()) {
        return NULL;
    }

    return items_[i];
}

void UiMenu::AddItem(UiMenuItem *item)
{
    items_.push_back(item);
}
