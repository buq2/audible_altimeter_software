#ifndef UI_CONFIG_ITEM_INT_HH
#define UI_CONFIG_ITEM_INT_HH

#include "ui_menu_item.hh"
#include "config.hh"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "common.hh"

static const uint8_t font_scale_x = 1;
static const uint8_t font_scale_y = 1;

template<class T>
class UiConfigItemInt
        :
        public UiMenuItem
{
 public:
    UiConfigItemInt(UiMenu *parent, const char *label, const T min_val, const T max_val)
        :
          UiMenuItem(parent),
          label_(label),
          min_val_(min_val),
          max_val_(max_val),
          selected_digit_(0)
    {
    }

    void SetPointer(T *ptr)
    {
        ptr_ = ptr;
    }

    const char *GetLabel()
    {
        return label_;
    }

    const char *GetLabelValue()
    {
        sprintf(label_value_, "%d", *ptr_);
        return label_value_;
    }

    void Render(DisplayBuffer *buffer)
    {
        buffer->Clear();

        const fontStyle_t &font = *(GetConfig()->GetIntChangeFont());
        const char *str = GetLabelValue();
        const uint8_t text_height = DisplayBuffer::CalculateTextHeightPixels(font,font_scale_y);
        const uint8_t text_width = DisplayBuffer::CalculateTextWidthPixels(font, font_scale_x, str);
        const uint8_t ypos = buffer->GetHeight()/2 - text_height/2;
        const uint8_t xpos = buffer->GetWidth()/2 - text_width/2;
        const uint8_t char_width = text_width/strlen(str);
        buffer->RenderText(font, xpos + text_width - char_width*(selected_digit_+1), ypos+text_height/2,font_scale_x, font_scale_y, "-");
        buffer->RenderText(font,
                           xpos, ypos,
                           font_scale_x, font_scale_y, str);
    }

    void KeyPress(const UiBase::KeyCode key, const bool down)
    {
        if (!down) {
            return;
        }

        switch(key) {
        case UiBase::KEY_UP:
        {
            int16_t new_val = (int16_t)*ptr_ + pow(10,selected_digit_);
            *ptr_ = MIN(new_val,max_val_);
            return;
        }
        case UiBase::KEY_DOWN:
        {
            int16_t new_val = (int16_t)*ptr_ - pow(10,selected_digit_);
            *ptr_ = MAX(new_val,min_val_);
            return;
        }
        case UiBase::KEY_RIGHT:
            ++selected_digit_;
            if (pow(10,selected_digit_) > max_val_) {
                selected_digit_ = 0;
            }
            return;
        default:
            break;
        }

        UiMenuItem::KeyPress(key,down);
    }
 private:
    T *ptr_;
    const char *label_;
    const T min_val_;
    const T max_val_;
    char label_value_[6];
    uint8_t selected_digit_;
}; //class UiConfigItemEnum

#endif //ifndef UI_CONFIG_ITEM_INT_HH
