#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "panel.h"

#define BRIGHTNESS 30
#define DELAY 10
#define INIT_VALUE 125

#define GRID_PIN 3
#define GRID_HEIGHT 8
#define GRID_LENGTH 32
#define TOP_STRIP_PIN 5
#define BOT_STRIP_PIN 6
#define STRIP_LENGTH 58


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    GRID_LENGTH,
    GRID_HEIGHT,
    GRID_PIN,
    NEO_MATRIX_RIGHT   + NEO_MATRIX_BOTTOM +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
    NEO_GRB            + NEO_KHZ800
);

Adafruit_NeoPixel top_strip = Adafruit_NeoPixel(
    STRIP_LENGTH,
    TOP_STRIP_PIN,
    NEO_GRB + NEO_KHZ800
);

Adafruit_NeoPixel bot_strip = Adafruit_NeoPixel(
    STRIP_LENGTH,
    BOT_STRIP_PIN,
    NEO_GRB + NEO_KHZ800
);

Panel panel = Panel(
    &matrix,
    &top_strip,
    &bot_strip
    );

void setup() {
  // put your setup code here, to run once:
    panel.init(BRIGHTNESS, INIT_VALUE);
}

void loop() {
  // put your main code here, to run repeatedly:
  panel.show();
  delay(DELAY);
}
