#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "../include/draw.h"
#include "../include/hardware_interface.h"

void test_draw_background(void) {
    uint8_t *buffer = hw_get_buffer();

    // Test black background
    draw_background(buffer, 320, 240, COLOR_BLACK);
    for (int i = 0; i < 320 * 240; i++) {
        assert(buffer[i] == COLOR_BLACK);
    }

    // Test white background
    draw_background(buffer, 320, 240, COLOR_WHITE);
    for (int i = 0; i < 320 * 240; i++) {
        assert(buffer[i] == COLOR_WHITE);
    }
}

void test_draw_rect(void) {
    uint8_t *buffer = hw_get_buffer();

    // Clear buffer
    draw_background(buffer, 320, 240, COLOR_BLACK);

    // Draw rectangle
    draw_rect(buffer, 320, 240, 10, 10, 20, 20, COLOR_RED);

    // Verify rectangle interior is filled correctly
    for (int y = 10; y < 30; y++) {
        for (int x = 10; x < 30; x++) {
            assert(buffer[y * 320 + x] == COLOR_RED);
        }
    }

    // Verify area outside rectangle is unchanged
    assert(buffer[0] == COLOR_BLACK);
    assert(buffer[320 * 240 - 1] == COLOR_BLACK);
}

static uint8_t test_character[FONT_HEIGHT * FONT_WIDTH] = {
      0,   0, 255, 255,   0,   0, 0, 0,
      0, 255, 255, 255, 255,   0, 0, 0,
    255, 255,   0,   0, 255, 255, 0, 0,
    255, 255,   0,   0, 255, 255, 0, 0,
    255, 255, 255, 255, 255, 255, 0, 0,
    255, 255,   0,   0, 255, 255, 0, 0,
    255, 255,   0,   0, 255, 255, 0, 0,
      0,   0,   0,   0,   0,   0, 0, 0,
};

void test_draw_text(void) {
    uint8_t *buffer = hw_get_buffer();

    // Clear buffer
    draw_background(buffer, 320, 240, COLOR_BLACK);

    // Draw text
    draw_text(buffer, 320, 240, 10, 10, "AAA", COLOR_WHITE);

    bool assert_failed = false;
    for (int i = 0; i < 3; i++) {
        for (int y = 0; y < FONT_HEIGHT; y++) {
            for (int x = 0; x < FONT_WIDTH; x++) {
                int pixel_x = x + 10 + i * FONT_WIDTH, pixel_y = y + 10;
                if (buffer[pixel_y * 320 + pixel_x] != test_character[y * FONT_WIDTH + x]) {
                    printf("buffer[%d][%d] = %x\n", y, x, buffer[pixel_y * 320 + pixel_x]);
                    assert_failed = true;
                }
            }
        }
    }
    assert(!assert_failed);
}

void test_draw_text_fast(void) {
    uint8_t *buffer = hw_get_buffer();

    // Clear buffer
    draw_background(buffer, 320, 240, COLOR_BLACK);

    // Draw text
    draw_text_fast(buffer, 320, 240, 10, 10, "AAA", COLOR_WHITE);

    bool assert_failed = false;
    for (int i = 0; i < 3; i++) {
        for (int y = 0; y < FONT_HEIGHT; y++) {
            for (int x = 0; x < FONT_WIDTH; x++) {
                int pixel_x = x + 10 + i * FONT_WIDTH, pixel_y = y + 10;
                if (buffer[pixel_y * 320 + pixel_x] != test_character[y * FONT_WIDTH + x]) {
                    printf("buffer[%d][%d] = %x\n", y, x, buffer[pixel_y * 320 + pixel_x]);
                    assert_failed = true;
                }
            }
        }
    }
    assert(!assert_failed);
}
