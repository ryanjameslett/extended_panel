#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#ifndef SPRITES
#include "sprites.h"
#define SPRITES
#endif

#define GRID_HEIGHT 8
#define GRID_LENGTH 32
#define STRIP_LENGTH 58


struct Color {
    long r, g, b;
};

class Panel
{
    private:
        Adafruit_NeoMatrix *_matrix;
        Adafruit_NeoPixel *_top_strand, *_bottom_strand;
        int _brightness;

    public:
        Panel(Adafruit_NeoMatrix *matrix,
              Adafruit_NeoPixel *top,
              Adafruit_NeoPixel *bottom
        );

        void init(int brightness, int init_r, int init_g, int init_b);
        void show();
        void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
        void setPixel(int x, int y, uint16_t color);
        void setCol(int x, uint8_t r, uint8_t g, uint8_t b);
        void fill(int r, int g, int b);
        void renderSprite(int x, int y, Sprite* sprite);
        Color getColor(int wheel);
        void setBrightness(byte value);
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

void Panel::init(int brightness, int init_r, int init_g, int init_b) {
    _brightness = brightness;

    _matrix->begin();
    _matrix->setBrightness(_brightness);
    _top_strand->begin();
    _top_strand->setBrightness(_brightness);
    _bottom_strand->begin();
    _bottom_strand->setBrightness(_brightness);

    uint16_t m_init_color = _matrix->Color(init_r, init_g, init_b);
    uint32_t s_init_color = _top_strand->Color(init_r, init_g, init_b);

    _matrix->fillScreen(m_init_color);
    _top_strand->fill(s_init_color);
    _bottom_strand->fill(s_init_color);
}


void Panel::setBrightness(byte value) {
    _matrix->setBrightness(value);
    _top_strand->setBrightness(value);
    _bottom_strand->setBrightness(value);
}

void Panel::fill(int r, int g, int b) {
    uint16_t m_color = _matrix->Color(r, g, b);
    uint32_t s_color = _top_strand->Color(r, g, b);
    _matrix->fillScreen(m_color);
    _bottom_strand->fill(s_color);
    _top_strand->fill(s_color);
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

void Panel::renderSprite(int x, int y, Sprite* sprite) {
    byte i,j;
    this->fill(sprite->bg_r, sprite->bg_g, sprite->bg_b);

    // draw the sprite
    for (i=0; i<8; i++) {
        for (j=0; j<8; j++) {
            if (sprite->shape[i][j]) {
                // flip X-axis
                this->setPixel(
                    x + (i * -1) + 7,
                    y + j,
                    sprite->r,
                    sprite->g,
                    sprite->b
                );
            }
        }
    }
}

void Panel::show() {
  _matrix->show();
  _top_strand->show();
  _bottom_strand->show();
}

Color Panel::getColor(int wheel) {
    wheel = 255 - wheel;
    if (wheel < 85) {
        return (Color){.r = 255 - wheel * 3, .g = 0, .b = wheel * 3};
    }
    if(wheel < 170) {
        wheel -= 85;
        return (Color){.r = 0, .g = wheel * 3, .b = 255 - wheel * 3};
    }
    wheel -= 170;
    return (Color){.r = wheel * 3, .g = 255 - wheel * 3, .b = 0};
}
