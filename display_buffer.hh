#ifndef DISPLAY_BUFFER_HH
#define DISPLAY_BUFFER_HH

#include <QPixmap>
#include <stdint.h>
#include "fonts/fontlibrary.h"

class DisplayBuffer
{
 public:
    DisplayBuffer(const uint8_t width, const uint8_t height);
    ~DisplayBuffer();

    QPixmap GetPixmap() const;
    int GetNumberOfBytes();
    int GetWidth();
    int GetHeight();
    void RenderText(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, char *str);
    void RenderText_AlignRight(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, char *str);
    void RenderText_AlignCenter(const fontStyle_t &font, const uint8_t x, const uint8_t y, const uint8_t scale_x, const uint8_t scale_y, char *str);
    static uint8_t CalculateTextWidthPixels(const fontStyle_t &font, const uint8_t scale_x, char *str);
    static uint8_t CalculateTextHeightPixels(const fontStyle_t &font, const uint8_t scale_y, char *str);
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
    uint8_t *data_;
}; //class DisplayBuffer

#endif //ifndef DISPLAY_BUFFER_HH
