#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include "panel.h"
#include "buttons.h"

#ifndef SPRITES
#include "sprites.h"
#define SPRITES
#endif

#define BRIGHTNESS 127
#define BRIGHTNESS_INCR 32
#define DELAY 10
#define INIT_COLOR_R 0
#define INIT_COLOR_G 0
#define INIT_COLOR_B 0

#define GRID_HEIGHT 8
#define GRID_LENGTH 32
#define STRAND_LENGTH 58

#define GRID_PIN 4
#define TOP_STRAND_PIN 2
#define BOT_STRAND_PIN 3

#define BUTTON_UP_PIN 11
#define BUTTON_DOWN_PIN 10
#define BUTTON_LEFT_PIN 8
#define BUTTON_RIGHT_PIN 9
#define BUTTON_NEXT_PIN 12
#define BUTTON_BRIGHTNESS_PIN 13

#define P_INIT 1
#define P_COLOR_WHEEL 0
#define P_COLOR_WIPE 1
#define P_RAIN 2
#define P_SIMPLE_SNAKE 3
#define P_RENDER_SPRITES 4
#define NUM_PROGRAMS 5

#define LEFT 0x1000
#define RIGHT 0x0100
#define UP 0x0010
#define DOWN 0x0001

#define P_TEST 10


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
int g_curr_program = P_INIT;

bool g_button_brightness_pressed = false;
byte g_curr_brightness = BRIGHTNESS;

Color color;

// global vars that can be used for various loops
// max value 256
byte x, y, i, j, d_pad;

/**
 * Test loop
 */
void test_loop() {
    uint8_t r,g,b;
    r=255;
    g=0;
    b=0;
    panel.fill(r, g, b);
    panel.show();
    delay(1000);

    r=0;
    g=255;
    b=0;
    panel.fill(r, g, b);
    panel.show();
    delay(1000);

    r=0;
    g=0;
    b=255;
    panel.fill(r, g, b);
    panel.show();
    delay(1000);
}

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

                // bail out on next program press
                if (pressed(BUTTON_NEXT_PIN)) {
                    return;
                }

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
bool wipe_forward = true;
#define WIPE_DELAY 10
void color_wipe_loop() {
    color = panel.getColor(random(0, 256));

    if (d_pad & DOWN == DOWN) {
        Serial.println("Down");
        wipe_forward = false;
        d_pad = 0;
    }
    else if (d_pad & UP == UP) {
        Serial.println("Up");
        wipe_forward = true;
        d_pad = 0;
    }

        panel.setCol(x, color.r, color.g, color.b);
        panel.setCol(x-1, color.r-(color.r/4), color.g-(color.g/4), color.b-(color.b/4));
        panel.setCol(x-2, color.r-(color.r/3), color.g-(color.g/3), color.b-(color.b/3));
        panel.setCol(x-3, color.r/2, color.g/2, color.b/2);
        panel.setCol(x-4, color.r/3, color.g/3, color.b/3);
        panel.setCol(x-5, color.r/4, color.g/4, color.b/4);
        panel.setCol(x-6, color.r/5, color.g/5, color.b/5);
        //panel.setCol(x-7, 0, 0, 0);
        panel.show();
        delay(10);
    }
}

/**
 * Rain Start
 */
int g_raindrop_size = 6;
int g_rain_counter = -1;

void rain_loop() {
    // Color color = panel.getColor(random(0, 256));
    if (g_rain_counter < 0) {
        g_rain_counter = random(0, 256);
    }
    Color color = panel.getColor(g_rain_counter);
    g_rain_counter += 5;
    if (g_rain_counter > 256) {
        g_rain_counter -= 256;
    }

    int r = color.r;
    int g = color.g;
    int b = color.b;
    int y = random(0, 8);
    // trail color

    for (int x = g_total_length; x >= -g_raindrop_size; x--) {
        // bail out on next program press
        if (pressed(BUTTON_NEXT_PIN)) {
            return;
        }

        panel.setPixel(x, y, r, g, b);
        panel.setPixel(x+1, y, r-(r/4), g-(g/4), b-(b/4));
        panel.setPixel(x+2, y, r-(r/3), g-(g/3), b-(b/3));
        panel.setPixel(x+3, y, r/2, g/2, b/2);
        panel.setPixel(x+4, y, r/3, g/3, b/3);
        panel.setPixel(x+5, y, r/4, g/4, b/4);
        // panel.setPixel(x+g_raindrop_size, y, tr_r, tr_g, tr_b);
        panel.show();
        delay(3);
    }
}

/**
 * Simple Snake
 */
int g_snake_size = 5;

void simple_snake_loop() {
    // snake color
    int r = random(0, 256);
    int g = random(0, 256 - r);
    int b = random(0, 256 - g);
    // trail color
    int tr_r = random(0, 256);
    int tr_g = random(0, 256 - tr_r);
    int tr_b = random(0, 256 - tr_g);
    int x, y;

    // move 1
    for (x = g_total_length; x >= -g_snake_size; x--) {
        // bail out on next program press
        if (pressed(BUTTON_NEXT_PIN)) {
            return;
        }
        panel.setPixel(x, 0, r, g, b);
        panel.setPixel(x+g_snake_size, 0, tr_r, tr_g, tr_b);
        panel.show();
        delay(5);
    }

    // move 2 - 7
    for (y = 1; y < GRID_HEIGHT - 1; y++) {
        for (x = 0; x < GRID_LENGTH + g_snake_size; x++) {
            // bail out on next program press
            if (pressed(BUTTON_NEXT_PIN)) {
                return;
            }
            panel.setPixel(x, y, r, g, b);
            panel.setPixel(x-g_snake_size, y, tr_r, tr_g, tr_b);
            panel.show();
            delay(5);
        }

        y++;

        for (x = GRID_LENGTH; x >= 0 - g_snake_size; x--) {
            // bail out on next program press
            if (pressed(BUTTON_NEXT_PIN)) {
                return;
            }
            panel.setPixel(x, y, r, g, b);
            panel.setPixel(x+g_snake_size, y, tr_r, tr_g, tr_b);
            panel.show();
            delay(5);
        }
    }

    // move 8
    for (x = 0; x < g_total_length + g_snake_size; x++) {
        // bail out on next program press
        if (pressed(BUTTON_NEXT_PIN)) {
            return;
        }
        panel.setPixel(x, 7, r, g, b);
        panel.setPixel(x-g_snake_size, 7, tr_r, tr_g, tr_b);
        panel.show();
        delay(5);
    }
}

/**
 * Render Sprites Loop
 */
void render_sprites_loop() {
    Sprite* sprite = get_rand_sprite();
    int x, y, xmin, xmax, ymin, ymax, buffer, buffer_sm;
    int dx, dy;
    int count, max_loops;

    dx = dy = 1;

    if (random(0, 10) > 4) {
        dx = -dx;
    }
    if (random(0, 10) > 4) {
        dy = -dy;
    }

    count = 0;
    max_loops = 200;

    buffer = 6;
    buffer_sm = -2;
    xmin = 0 - buffer;
    xmax = GRID_LENGTH + buffer_sm;
    ymin = 0 - buffer;
    ymax = GRID_HEIGHT + buffer_sm;

    x = random(xmin, xmax);
    y = random(ymin, ymax);

    while(count < max_loops) {
        // bail out on next program press
        if (pressed(BUTTON_NEXT_PIN)) {
            return;
        }

        x += dx;
        y += dy;

        /*
        Serial.print(x);
        Serial.print(",");
        Serial.println(y);
        */

        if (x < xmin || x > xmax) {
            dx = -dx;
        }

        if (y < ymin || y > ymax) {
            dy = -dy;
        }

        panel.renderSprite(x, y, sprite);
        panel.show();
        delay(80);
        count++;
    }
}

/**
 *Main code
 */

/**
 * This code updates things during loops so that we can get button presses
 * and do things like update brightness from within loops
 * Returns true if the program needs to exit
 */
bool interrupt() {
    if (digitalRead(BUTTON_NEXT_PIN) == LOW) {
        return true;
    }

    update_d_pad();
    update_brightness(); 

    return false;
}

void update_d_pad() {
    if (digitalRead(BUTTON_LEFT_PIN) == LOW) {
        d_pad = d_pad | LEFT;
    }
    if (digitalRead(BUTTON_RIGHT_PIN) == LOW) {
        d_pad = d_pad | RIGHT;
    }
    if (digitalRead(BUTTON_UP_PIN) == LOW) {
        d_pad = d_pad | UP;
    }
    if (digitalRead(BUTTON_DOWN_PIN) == LOW) {
        d_pad = d_pad | DOWN;
    }
    // Serial.println(d_pad);
}

void update_brightness() {
    if (digitalRead(BUTTON_BRIGHTNESS_PIN) == LOW) {
        g_button_brightness_pressed = true;
    }
    else if (g_button_brightness_pressed) {
        g_curr_brightness = g_curr_brightness + BRIGHTNESS_INCR;
        if (g_curr_brightness > 256) {
            g_curr_brightness -+ 256;
        }
        panel.setBrightness(g_curr_brightness);
        g_button_brightness_pressed = false;
    }
}

void setup() {
    Serial.begin(9600);
    // Serial.println("Setup");
    panel.init(g_curr_brightness, INIT_COLOR_R, INIT_COLOR_G, INIT_COLOR_B);

    pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
    pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
    pinMode(BUTTON_LEFT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_NEXT_PIN, INPUT_PULLUP);

    panel.show();
    delay(200);
}

void loop() {
    // Serial.println("Loop");
    if (pressed(BUTTON_NEXT_PIN)) {
        g_curr_program++;
        delay(200);
        panel.fill(0, 0, 0);
        if (g_curr_program >= NUM_PROGRAMS) {
            g_curr_program = 0;
        }
    }

    switch (g_curr_program) {
        case P_COLOR_WHEEL:
            color_wheel_loop();
            break;

        case P_COLOR_WIPE:
            color_wipe_loop();
            break;

        case P_RAIN:
            rain_loop();
            break;

        case P_SIMPLE_SNAKE:
            simple_snake_loop();
            break;

        case P_RENDER_SPRITES:
            render_sprites_loop();
            break;

        // Test and diagnostic programs
        case P_TEST:
            test_loop();
            break;

    }
}
