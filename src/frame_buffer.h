#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include <stdint.h>

// Frame buffer dimensions
#define FRAME_WIDTH 360
#define FRAME_HEIGHT 240

// Initialize frame buffers
void frame_buffer_init(void);

// Get pointer to the current display buffer (being displayed)
const uint8_t* frame_buffer_get_display(void);

// Get pointer to the current draw buffer (being drawn to)
uint8_t* frame_buffer_get_draw(void);

// Get pointer to specific line in display buffer
const uint8_t* frame_buffer_get_line(uint32_t line);

// Clear draw buffer with specific color
void frame_buffer_clear(uint8_t color);

// Draw a pixel in the draw buffer
void frame_buffer_draw_pixel(uint32_t x, uint32_t y, uint8_t color);

// Swap display and draw buffers
void frame_buffer_swap(void);

#endif // _FRAME_BUFFER_H_
