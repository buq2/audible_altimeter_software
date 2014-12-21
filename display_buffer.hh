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
    void RenderText(const fontStyle_t &font, const uint8_t x, const uint8_t y, char *str);

    typedef enum PixelManipulate_t {
        PIXEL_SET,
        PIXEL_CLEAR,
        PIXEL_TOGGLE
    } PixelManipulate;
    void ModifyPixel(const uint8_t x, const uint8_t y, const PixelManipulate op);
    bool GetPixel(const uint8_t x, const uint8_t y);
    void BlitRow(const uint8_t x, const uint8_t y, const uint8_t *data, const uint8_t width_bits);
    void Clear();
 private:
    uint8_t width_;
    uint8_t height_;
    uint8_t stride_;
    uint8_t *data_;
}; //class DisplayBuffer

#endif //ifndef DISPLAY_BUFFER_HH
