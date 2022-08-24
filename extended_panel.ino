#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "panel.h"

#define PROGRAM 2
#define BRIGHTNESS 10
#define DELAY 10
#define INIT_COLOR 0

#define GRID_HEIGHT 8
#define GRID_LENGTH 32
#define STRAND_LENGTH 58

#define GRID_PIN 3
#define TOP_STRAND_PIN 5
#define BOT_STRAND_PIN 6

#define P_COLOR_WHEEL 1
#define P_COLOR_WIPE 2


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
int g_total_length = (GRID_LENGTH + STRAND_LENGTH);
int g_total_num_pixels = (GRID_LENGTH + STRAND_LENGTH) * GRID_HEIGHT;

/***
 * Color Wheel Start
 */
byte g_color_counter = 0;
int increment = g_total_num_pixels / 255.0;

void set_color_pixel(int x, int y, byte value) {
    if (value < 85) {
        panel.setPixel(x, y, value * 3, 255 - value, 0);
    }
    else if (value < 170) {
        value = value - 85;
        panel.setPixel(x, y, 255 - value, 0, value * 3);
    }
    else {
        value = value - 170;
        panel.setPixel(x, y, 0, value * 3, 255 - (value * 3));
    }
}

void color_wheel_loop() {
    for (int16_t j = 0; j < 256 * 5; j++) {
        for (int16_t x = 0; x < GRID_LENGTH + STRAND_LENGTH ; x++) {
            for (int16_t y = 0; y < GRID_HEIGHT; y++) {
                byte tmp = 0;

                tmp = ((g_color_counter * 256 / g_total_length) + j) & 255;
                set_color_pixel(x, y, tmp);

                g_color_counter = g_color_counter + increment;
                if (g_color_counter > 255) {
                    g_color_counter = 0;
                }
            }
        }
        panel.show();
        delay(50);
    }
}

/***
 * Color Wipe Start
 */

void color_wipe_loop() {
    for (int count = 0; count < 3; count++) {
        for (int x = 0; x < g_total_length; x++) {
            if (count % 3 == 0) {
                panel.setCol(x, 255, 0, 0);
            }
            else if (count % 3 == 1) {
                panel.setCol(x, 0, 255, 0);
            }
            else {
                panel.setCol(x, 0, 0, 255);
            }
            panel.show();
            delay(20);
        }
    }
}

/**
 *Main code
 */

void setup() {
    Serial.begin(9600);
    panel.init(BRIGHTNESS, INIT_COLOR);
}

void loop() {
    Serial.println("Loop");
    switch (PROGRAM) {
        case P_COLOR_WHEEL:
            color_wheel_loop();
            break;

        case P_COLOR_WIPE:
            color_wipe_loop();
            break;
    }
}
