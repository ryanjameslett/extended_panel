#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "panel.h"

#define BRIGHTNESS 30
#define DELAY 10
#define INIT_VALUE 100

#define GRID_HEIGHT 8
#define GRID_LENGTH 32
#define STRAND_LENGTH 58

#define GRID_PIN 3
#define TOP_STRAND_PIN 5
#define BOT_STRAND_PIN 6


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    GRID_LENGTH,
    GRID_HEIGHT,
    GRID_PIN,
    NEO_MATRIX_RIGHT   + NEO_MATRIX_BOTTOM +
    NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
    NEO_GRB            + NEO_KHZ800
);

Adafruit_NeoPixel top_strand = Adafruit_NeoPixel(
    STRAND_LENGTH,
    TOP_STRAND_PIN,
    NEO_GRB + NEO_KHZ800
);

Adafruit_NeoPixel bottom_strand = Adafruit_NeoPixel(
    STRAND_LENGTH,
    BOT_STRAND_PIN,
    NEO_GRB + NEO_KHZ800
);

Panel panel = Panel(
    &matrix,
    &top_strand,
    &bottom_strand
    );

// Program Globals
int g_loop = 0;

void setup() {
    Serial.begin(9600);
    // panel.init(BRIGHTNESS, INIT_VALUE);
}

void loop() {
  Serial.println("Loop");

  /*
  for (int16_t x = 0; x < GRID_LENGTH + STRIP_LENGTH ; x++) {
    for (int16_t y = 0; y < GRID_HEIGHT; y++) {
        uint32_t color;
        if (x < GRID_LENGTH) {
            color = matrix.Color(255, 0, 0);
        }
        else {
            color = top_strand.Color(255, 0, 0);
        }
        Serial.println(color);
        panel.setPixel(x, y, color);
    }
  }
  */
  // panel.setPixel(0, 1, matrix.Color(255, 0, 0));
  matrix.drawPixel(0, 3, matrix.Color(255, 0, 0));
  top_strand.setPixelColor(34, 255, 0, 0);
  bottom_strand.setPixelColor(34, 255, 0, 0);
  matrix.show();
  top_strand.show();
  bottom_strand.show();

  delay(DELAY);
  // panel.show();

  g_loop++;
}
