#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define GRID_HEIGHT 8
#define GRID_LENGTH 32
#define STRIP_LENGTH 58

/**
 * TODO:
 * light rows
 * light cols
 */

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
        void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
        void setPixel(int x, int y, uint16_t color);
        void setCol(int x, uint8_t r, uint8_t g, uint8_t b);
        void fill(uint16_t color);
        void renderSprite(int x, int y, bool sprite[8][8], uint8_t r, uint8_t g, uint8_t b);
};

Panel::Panel(
        Adafruit_NeoMatrix *matrix,
        Adafruit_NeoPixel *top,
        Adafruit_NeoPixel *bottom
) {
    _matrix = matrix;
    _top_strand = top;
    _bottom_strand = bottom;
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

void Panel::fill(uint16_t color) {
    _matrix->fillScreen(_init_value);
    _top_strand->fill(_init_value);
    _bottom_strand->fill(_init_value);
}

void Panel::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x < GRID_LENGTH) {
        _matrix->drawPixel(x, y, _matrix->Color(r,g,b));
    }
    else {
        if (y == 0) {
            _top_strand->setPixelColor(x-GRID_LENGTH, _top_strand->Color(r,g,b));
        }
        else if (y == 7) {
            _bottom_strand->setPixelColor(x-GRID_LENGTH, _bottom_strand->Color(r,g,b));
        }
    }
}

void Panel::setPixel(int x, int y, uint16_t color) {
    if (x < GRID_LENGTH) {
        _matrix->drawPixel(x, y, color);
    }
    else {
        if (y == 0) {
            _top_strand->setPixelColor(x-GRID_LENGTH, color);
        }
        else if (y == 7) {
            _bottom_strand->setPixelColor(x-GRID_LENGTH, color);
        }
    }
}

void Panel::setCol(int x, uint8_t r, uint8_t g, uint8_t b) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        Panel::setPixel(x, y, r, g, b);
    }
}

void Panel::renderSprite(int x, int y, bool sprite[8][8], uint8_t r, uint8_t g, uint8_t b) {

}

void Panel::show() {
  _matrix->show();
  _top_strand->show();
  _bottom_strand->show();
}
