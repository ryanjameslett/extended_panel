#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>


class Panel
{
    private:
        Adafruit_NeoMatrix *_matrix;
        Adafruit_NeoPixel *_top_strand, *_bottom_strand;
        int _brightness, _init_value;

    public:
        Panel(Adafruit_NeoMatrix *matrix,
              Adafruit_NeoPixel *top,
              Adafruit_NeoPixel *bottom
        );

        void init(int brightness, int init_value);
        void show();
};

Panel::Panel(
        Adafruit_NeoMatrix *matrix,
        Adafruit_NeoPixel *top,
        Adafruit_NeoPixel *bottom
) {
    _matrix = matrix;
    *_top_strand = *top;
    *_bottom_strand = *bottom;
}

void Panel::init(int brightness, int init_value) {
    _brightness = brightness;
    _init_value = init_value;

    _matrix->begin();
    _matrix->setBrightness(_brightness);
    _top_strand->begin();
    _top_strand->setBrightness(_brightness);
    _bottom_strand->begin();
    _bottom_strand->setBrightness(_brightness);
    _matrix->fillScreen(_init_value);
    _top_strand->fill(_init_value);
    _bottom_strand->fill(_init_value);
}

void Panel::show() {
  _matrix->show();
  _top_strand->show();
  _bottom_strand->show();
}
