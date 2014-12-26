#include "display_buffer.hh"
#include <string.h>
#include "common.hh"

const bool flip_fonts = true;
const uint8_t glyph_spacing_pixels = 2;

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
      stride_(width_/8)
{
    Clear();
}

DisplayBuffer::~DisplayBuffer()
{
}

uint8_t *DisplayBuffer::GetBuffer()
{
    return data_;
}

uint16_t DisplayBuffer::GetNumberOfBytes() const
{
    return width_*height_/8;
}

uint8_t DisplayBuffer::GetWidth() const
{
    return width_;
}

uint8_t DisplayBuffer::GetHeight() const
{
    return height_;
}

void DisplayBuffer::RenderText(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, const char *str)
{
    uint8_t current_x = x;

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
        BlitGraphics(current_x, y, scale_x, scale_y, width_bits, font.GlyphBytesWidth, height, ptr_font);

        current_x += width_bits*scale_x + glyph_spacing_pixels;
    }
}

void DisplayBuffer::RenderText_AlignRight(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, const char *str)
{
    const uint8_t width_bits = CalculateTextWidthPixels(font, scale_x, str);
    RenderText(font, x - width_bits, y, scale_x, scale_y, str);
}

void DisplayBuffer::RenderText_AlignCenter(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, const char *str)
{
    const uint8_t width_bits = CalculateTextWidthPixels(font, scale_x, str);
    RenderText(font, x - width_bits/2, y, scale_x, scale_y, str);
}

uint8_t DisplayBuffer::CalculateTextWidthPixels(const fontStyle_t &font, const uint8_t scale_x, const char *str)
{
    // Calculate text full width in bits
    uint8_t text_width_bits = 0;
    const int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        const uint8_t c = str[i];
        const uint8_t pos = c-font.FirstAsciiCode;
        if (c < font.FirstAsciiCode || pos >= font.GlyphCount) {
            // Can not render
            continue;
        }

        const uint8_t width_bits = font.GlyphWidth[pos];
        text_width_bits += width_bits*scale_x;
        if (i < len-1) {
            text_width_bits += glyph_spacing_pixels;
        }
    }

    return text_width_bits;
}

uint8_t DisplayBuffer::CalculateTextHeightPixels(const fontStyle_t &font, const uint8_t scale_y, const char */*str*/)
{
    // Currently not using str
    return font.GlyphHeight*scale_y;
}

uint8_t DisplayBuffer::CalculateTextHeightPixels(const fontStyle_t &font, const uint8_t scale_y)
{
    const char *str = "";
    return CalculateTextHeightPixels(font, scale_y, str);
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

void DisplayBuffer::BlitGraphics(const uint8_t x, const uint8_t y,
                                 const uint8_t scale_x, const uint8_t scale_y,
                                 const uint8_t width_bits, const uint8_t stride_bytes,
                                 const uint8_t height_rows, const uint8_t *data)
{
    for (uint8_t row = 0; row < height_rows; ++row) {
        const uint8_t *ptr_row = &data[stride_bytes*row];
        for (uint8_t scale_y_idx = 0; scale_y_idx < scale_y; ++scale_y_idx) {
            BlitRow(x,y + row*scale_y + scale_y_idx, scale_x, ptr_row, width_bits);
        }
    }
}

void DisplayBuffer::BlitRow(const uint8_t x, const uint8_t y, const uint8_t *pixels, const uint8_t width_bits)
{
    BlitRow(x, y, 1, pixels, width_bits);
}

void DisplayBuffer::BlitRow(const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t *pixels, const uint8_t width_bits)
{
    if (x >= width_ || y >= height_) {
        return;
    }

    // Slow
    uint8_t bit = 0;
    for (uint8_t i = 0; i < width_bits/8 + 1; ++i) {
        // Get single byte
        // NOTE: Currently all graphics must be in flash memory of the MCU
        const uint8_t byte = pgm_read_byte(pixels + i);
        for (uint8_t j = 0; j < 8 && bit < width_bits; ++j) {
            const bool val = byte & (1 << j);
            for (uint8_t scale_x_idx = 0; scale_x_idx < scale_x; ++scale_x_idx) {
                const uint8_t x_pos = x + bit*scale_x + scale_x_idx;
                if (val) {
                    ModifyPixel(x_pos,y,PIXEL_SET);
                } else {
                    ModifyPixel(x_pos,y,PIXEL_CLEAR);
                }
            }
            ++bit;
        }
    }
}

void DisplayBuffer::Clear()
{
    memset(data_,0,GetNumberOfBytes());
}
