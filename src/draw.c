#include <stdint.h>
#include <string.h>

#include "draw.h"

#include "../vendor/font8x8/font8x8_basic.h"

// Draw background
void draw_background(uint8_t *buffer, int width, int height, int color) {
    memset(buffer, color, width * height);
}

// Draw rectangle
void draw_rect(uint8_t *buffer, int buffer_width, int buffer_height, int x, int y, int width, int height, int color) {
    // Boundary check
    if (x < 0 || y < 0 || x >= buffer_width || y >= buffer_height) {
        return;
    }

    int draw_width = (x + width > buffer_width) ? buffer_width - x : width;
    int draw_height = (y + height > buffer_height) ? buffer_height - y : height;
    for (int j = 0; j < draw_height; j++) {
        memset(&buffer[(y + j) * buffer_width + x], color, draw_width);
    }
}

// Draw text line
void draw_text(uint8_t *buffer, int width, int height, int x, int y, const char *text, int color) {
    if (y + FONT_HEIGHT <= 0 || y >= height || x >= width) {
        return;
    }

    for (int t = 0; text[t] != '\0'; t++) {
        unsigned char c = (unsigned char)text[t];
        const uint8_t *glyph = font8x8_basic[c];
        if (!glyph) continue;

        int char_x = x + t * FONT_WIDTH;
        if (char_x + FONT_WIDTH <= 0 || char_x >= width) continue;

        for (int j = 0; j < FONT_HEIGHT; j++) {
            int py = y + j;
            if (py < 0 || py >= height) continue;

            uint8_t bits = glyph[j];
            for (int i = 0; i < FONT_WIDTH; i++) {
                int px = char_x + i;
                if (px < 0 || px >= width) continue;

                if (bits & (1 << i)) {
                    buffer[py * width + px] = color;
                }
            }
        }
    }
}

// Draw text line faster (skips coordinate checks)
void draw_text_fast(uint8_t *buffer, int width, int height, int x, int y, const char *text, int color) {
    for (int i = 0; text[i] != '\0'; i++) {
        const uint8_t *glyph = (uint8_t *)font8x8_basic[(uint8_t)text[i]];
        int base_x = x + i * FONT_WIDTH;

        for (int dy = 0; dy < FONT_HEIGHT; dy++) {
            uint8_t bits = glyph[dy];
            uint8_t *dst = &buffer[(y + dy) * width + base_x];

            for (int dx = 0; dx < FONT_WIDTH; dx++) {
                if (bits & (1 << dx)) {
                    dst[dx] = color;
                }
            }
        }
    }
}
