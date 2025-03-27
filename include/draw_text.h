#ifndef _DRAW_TEXT_H_
#define _DRAW_TEXT_H_

#include <stdint.h>

#define FONT_WIDTH 8
#define FONT_HEIGHT 8

// Function declarations
void draw_text_line(uint8_t *line_buf, int line_buf_width, int line_y, int x, int y, const char *text, int color);

#endif
