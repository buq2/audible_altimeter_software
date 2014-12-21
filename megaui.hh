#ifndef MEGAUI_HH
#define MEGAUI_HH

#include <QPixmap>
#include <stdint.h>
#include "fonts/fontlibrary.h"

class Megaui
{
public:
    Megaui(const uint8_t width, const uint8_t height);
    ~Megaui();
    QPixmap GetPixmap() const;
    int GetNumberOfBytes();
    int GetWidth();
    int GetHeight();
    void RenderText(const uint8_t x, const uint8_t y, char *str);

    typedef enum PixelManipulate_t {
        PIXEL_SET,
        PIXEL_CLEAR,
        PIXEL_TOGGLE
    } PixelManipulate;
    void ModifyPixel(const uint8_t x, const uint8_t y, const PixelManipulate op);
    bool GetPixel(const uint8_t x, const uint8_t y);
    void BlitRow(const uint8_t x, const uint8_t y, const uint8_t *data, const uint8_t width_bits);

    void KeyUp(const bool pressed);
    void KeyDown(const bool pressed);
    void KeyLeft(const bool pressed);
    void KeyRight(const bool pressed);
private:
    uint8_t width_;
    uint8_t height_;
    uint8_t stride_;
    uint8_t *data_;
    fontStyle_t *font_;
};

#endif // MEGAUI_HH
