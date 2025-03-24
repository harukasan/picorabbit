#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <stdint.h>

// Color constants (8-bit RGB332 format)
#define COLOR_BLACK   0x00
#define COLOR_RED     0xE0  // 111 000 00
#define COLOR_GREEN   0x1C  // 000 111 00
#define COLOR_BLUE    0x03  // 000 000 11
#define COLOR_WHITE   0xFF  // 111 111 11
#define COLOR_YELLOW  0xFC  // 111 111 00
#define COLOR_MAGENTA 0xE3  // 111 000 11
#define COLOR_CYAN    0x1F  // 000 111 11

// Convert RGB values (0-7 for R and G, 0-3 for B) to RGB332 format
static inline uint8_t RGB332(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0x07) << 5) | ((g & 0x07) << 2) | (b & 0x03);
}

// Initialize text renderer
void text_renderer_init(void);

// Render text at specified position
// x, y: position in characters (not pixels)
// text: null-terminated string to render
// color: color value in RGB332 format
void render_text(uint8_t x, uint8_t y, const char* text, uint8_t color);

// Clear text at specified position
void clear_text(uint8_t x, uint8_t y, uint8_t length);

// Get pointer to the specified line in the text buffer
const uint8_t* get_text_line(uint32_t line);

#endif // TEXT_RENDERER_H
