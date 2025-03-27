#include <stdint.h>
#include "draw_text.h"
#include "../vendor/font8x8/font8x8_basic.h"

// Draw text line
void draw_text_line(uint8_t *line_buf, int line_buf_width, int line_y, int x, int y, const char *text, int color) {
    int rel_y = line_y - y;

    // Skip if the character is out of the line buffer (top or bottom)
    if (rel_y < 0 || rel_y >= FONT_HEIGHT) {
        return;
    }

    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        const uint8_t *glyph = font8x8_basic[c];
        if (!glyph) {
            continue;
        }

        // Draw each pixel of the character
        uint8_t bits = glyph[rel_y];
        for (int j = 0; j < FONT_WIDTH; j++) {
            int pixel_x = x + i * FONT_WIDTH + j;

            // Skip if the pixel is out of the line buffer (left or right)
            if (pixel_x < 0 || pixel_x >= line_buf_width) {
                continue;
            }
            if (bits & (1 << j)) {
                line_buf[pixel_x] = color;
            }
        }
    }
}
