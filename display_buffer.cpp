#include "display_buffer.hh"

#include <QImage>
#include <cstring>
#include <iostream>

const bool flip_fonts = true;

uint8_t bit_flip_lookup[16] = {
   0x0, 0x8, 0x4, 0xC,
   0x2, 0xA, 0x6, 0xE,
   0x1, 0x9, 0x5, 0xD,
   0x3, 0xB, 0x7, 0xF };

uint8_t BitFlip(uint8_t n )
{
   // http://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
   //This should be just as fast and it is easier to understand.
   //return (lookup[n%16] << 4) | lookup[n/16];
   return (bit_flip_lookup[n&0x0F] << 4) | bit_flip_lookup[n>>4];
}

DisplayBuffer::DisplayBuffer(const uint8_t width, const uint8_t height)
    :
      width_(width),
      height_(height),
      data_(new uint8_t[width_*height_/8]),
      stride_(width_/8)
{
    memset(data_,0,GetNumberOfBytes());
    font_ = &FontStyle_impact_huge;
    RenderText(3,0,"12345");
    font_ = &FontStyle_impact;
    RenderText(3,40,"Descent: 26 ft/");
    RenderText(3,55,"Alt  : 390");
    RenderText(3,70,"Climb: 1150");
    //RenderText(0,0,"Test test");
    //RenderText(0,19,"Test test");
    //RenderText(0,19*2,"Test test");
    //RenderText(0,19*3,"Test test");
    //RenderText(0,19*4,"Test test");
    //for (int i = 0; i < 8; ++i) {
    //    ModifyPixel(i,i,PIXEL_SET);
    //}
}

DisplayBuffer::~DisplayBuffer()
{
    delete data_;
}

QPixmap DisplayBuffer::GetPixmap() const
{
    QImage img((uchar*)data_,width_, height_,QImage::Format_MonoLSB);
    return QPixmap::fromImage(img);
}

int DisplayBuffer::GetNumberOfBytes()
{
    return width_*height_/8;
}

int DisplayBuffer::GetWidth()
{
    return width_;
}

int DisplayBuffer::GetHeight()
{
    return height_;
}

void DisplayBuffer::RenderText(const uint8_t x, const uint8_t y, char *str)
{
    uint8_t current_x = x;

    fontStyle_t font = *font_;

    const int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        const uint8_t c = str[i];
        const uint8_t pos = c-font.FirstAsciiCode;
        if (c < font.FirstAsciiCode || pos >= font.GlyphCount) {
            // Can not render
            continue;
        }

        // Stride of all the fonts
        const int font_stride = font.GlyphBytesWidth*font.GlyphHeight;
        const uint8_t *ptr_font = (uint8_t *)&font.GlyphBitmaps[pos*font_stride];

        const uint8_t width_bits = font.GlyphWidth[pos];
        const uint8_t height = font.GlyphHeight;
        for (uint8_t font_y = 0; font_y < height; ++font_y) {
            const uint8_t *ptr_font_row = &ptr_font[font.GlyphBytesWidth*font_y];
            BlitRow(current_x,y + font_y, ptr_font_row, width_bits);
        }

        current_x += width_bits + 2;
    }
}

void DisplayBuffer::ModifyPixel(const uint8_t x, const uint8_t y, const PixelManipulate op)
{
    if (x >= width_ || y >= height_) {
        return;
    }
    uint8_t *row_ptr = &data_[y*stride_];
    uint8_t *byte_ptr = &row_ptr[x/8];
    uint8_t data = *byte_ptr;
    const uint8_t bit = x % 8;
    switch(op) {
    case PIXEL_SET:
        data |= 1 << bit;
        break;
    case PIXEL_CLEAR:
        data &= ~(1 << bit);
        break;
    case PIXEL_TOGGLE:
        data ^= 1 << bit;
        break;
    }
    *byte_ptr = data;
}

bool DisplayBuffer::GetPixel(const uint8_t x, const uint8_t y)
{
    if (x >= width_ || y >= height_) {
        return false;
    }
    uint8_t *row_ptr = &data_[y*stride_];
    uint8_t *byte_ptr = &row_ptr[x/8];
    uint8_t data = *byte_ptr;
    const uint8_t bit = x % 8;
    return data & (1 << bit);
}

void DisplayBuffer::BlitRow(const uint8_t x, const uint8_t y, const uint8_t *pixels, const uint8_t width_bits)
{
    if (x >= width_ || y >= height_) {
        return;
    }

    // Slow
    uint8_t bit = 0;
    for (int i = 0; i < width_bits/8 + 1; ++i) {
        // Get single byte
        const uint8_t byte = pixels[i];
        for (int j = 0; j < 8 && bit < width_bits; ++j) {
            const bool val = byte & (1 << j);
            const uint8_t x_pos = x + bit;
            if (val) {
                ModifyPixel(x_pos,y,PIXEL_SET);
            } else {
                ModifyPixel(x_pos,y,PIXEL_CLEAR);
            }
            ++bit;
        }
    }
}
