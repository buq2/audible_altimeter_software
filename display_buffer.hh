#ifndef DISPLAY_BUFFER_HH
#define DISPLAY_BUFFER_HH

#include <stdint.h>
#include "fonts/fontlibrary.h"

class DisplayBuffer
{
 public:
    DisplayBuffer(const uint8_t width, const uint8_t height);
    ~DisplayBuffer();

    const uint8_t *GetBuffer() const;
    const uint8_t *GetBufferRow(const uint8_t &row) const;
    uint16_t GetNumberOfBytes() const;
    uint8_t GetWidth() const;
    uint8_t GetHeight() const;
    void RenderText(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, const char *str);
    void RenderText_AlignRight(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, const char *str);
    void RenderText_AlignCenter(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, const char *str);
    static uint8_t CalculateTextWidthPixels(const fontStyle_t &font, const uint8_t scale_x, const char *str);
    static uint8_t CalculateTextHeightPixels(const fontStyle_t &font, const uint8_t scale_y, const char *str);
    static uint8_t CalculateTextHeightPixels(const fontStyle_t &font, const uint8_t scale_y);
    typedef enum PixelManipulate_t {
        PIXEL_SET,
        PIXEL_CLEAR,
        PIXEL_TOGGLE
    } PixelManipulate;
    void ModifyPixel(const uint8_t x, const uint8_t y, const PixelManipulate op);
    bool GetPixel(const uint8_t x, const uint8_t y);
    void BlitGraphics(const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y,
                      const uint8_t width_bits, const uint8_t stride_bytes, const uint8_t height_rows,
                      const uint8_t *data);
    void BlitRow(const uint8_t x, const uint8_t y, const uint8_t *data, const uint8_t width_bits);
    void BlitRow(const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t *data, const uint8_t width_bits);
    void Clear();
 private:
    uint8_t width_;
    uint8_t height_;
    uint8_t stride_;
    uint8_t data_[128*128/8];
}; //class DisplayBuffer

#endif //ifndef DISPLAY_BUFFER_HH
