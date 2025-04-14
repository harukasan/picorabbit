#ifndef _DRAW_TEXT_H_
#define _DRAW_TEXT_H_

#include <stdint.h>

#define FONT_WIDTH 8
#define FONT_HEIGHT 8

#define COLOR_BLACK 0x00
#define COLOR_WHITE 0xFF
#define COLOR_RED 0xE0
#define COLOR_GREEN 0x1C
#define COLOR_BLUE 0x03
#define COLOR_YELLOW 0xFC
#define COLOR_CYAN 0x1F
#define COLOR_MAGENTA 0xE3

void draw_background(uint8_t *buffer, int color);
void draw_rect(uint8_t *buffer, int x, int y, int width, int height, int color);
void draw_text(uint8_t *buffer, int x, int y, const char *text, int color);
void draw_text_fast(uint8_t *buffer, int x, int y, const char *text, int color);
void draw_image(uint8_t *buffer, const uint8_t *image_data, int x, int y, int width, int height);
void draw_image_masked(uint8_t *buffer, const uint8_t *image_data, const uint8_t *mask_data, int x, int y, int width, int height);

#endif
