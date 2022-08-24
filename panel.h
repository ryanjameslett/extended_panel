#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>


class Panel
{
    private:
        Adafruit_NeoMatrix *_matrix;
        Adafruit_NeoPixel *_top_strand, *_bottom_strand;

    public:
        Panel();
        Panel(Adafruit_NeoMatrix *matrix, Adafruit_NeoPixel *top, Adafruit_NeoPixel *bottom);
        void show();
}

/**
 * Empty Constructor
 */
Panel::Panel() {}

/**
 * Construct a panel
 */
Panel::Panel(
        Adafruit_NeoMatrix *matrix,
        Adafruit_NeoPixel *top,
        Adafruit_NeoPixel *bottom,
        int _brightness
) {
    *_matrix = *matrix;
    *_top_strand = *top;
    *_bottom_strand = *bottom;
    _brightness = brightness;
}

void Panel::show() {
    _matrix.begin();
    _matrix.setBrightness(_brightness);
    _top_strand.begin();
    _top_strand.setBrightness(_brightness);
    _bottom_strand.begin();
    _bottom_strand.setBrightness(_brightness);
}

