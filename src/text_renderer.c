#include "text_renderer.h"
#include "../vendor/font8x8/font8x8_basic.h"
#include "dvi.h"
#include <string.h>

#define TEXT_COLS (DVI_H_ACTIVE / 8)
#define TEXT_ROWS (DVI_V_ACTIVE / 8)

// Store only character and color information
static struct {
    char character;
    uint8_t color;
} text_buffer[TEXT_ROWS][TEXT_COLS];

// Single line buffer for rendering
static uint8_t line_buffer[DVI_H_ACTIVE];

void text_renderer_init(void) {
    memset(text_buffer, 0, sizeof(text_buffer));
}

void render_text(uint8_t x, uint8_t y, const char* text, uint8_t color) {
    while (*text && x < TEXT_COLS) {
        text_buffer[y][x].character = *text;
        text_buffer[y][x].color = color;
        text++;
        x++;
    }
}

void clear_text(uint8_t x, uint8_t y, uint8_t length) {
    for (int i = 0; i < length && x + i < TEXT_COLS; i++) {
        text_buffer[y][x + i].character = ' ';
        text_buffer[y][x + i].color = 0;
    }
}

// Function to get the current line buffer for DVI output
const uint8_t* get_text_line(uint32_t line) {
    // Calculate which character row we're in and which line within that character
    uint32_t char_row = line / 8;
    uint32_t char_line = line % 8;

    // Clear line buffer
    memset(line_buffer, 0, sizeof(line_buffer));

    // If we're beyond text area, return empty line
    if (char_row >= TEXT_ROWS) {
        return line_buffer;
    }

    // Render the line
    for (uint32_t col = 0; col < TEXT_COLS; col++) {
        char c = text_buffer[char_row][col].character;
        uint8_t color = text_buffer[char_row][col].color;

        if (c && color) {
            // Get the font data for this character and line
            uint8_t font_line = font8x8_basic[c][char_line];

            // Copy the bits to the line buffer
            for (int bit = 0; bit < 8; bit++) {
                if (font_line & (1 << bit)) {
                    line_buffer[col * 8 + bit] = color;
                }
            }
        }
    }

    return line_buffer;
}
