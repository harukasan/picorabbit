#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#include "dvi_defines.h"

// Display resolution configuration
#define FRAMEBUFFER_WIDTH 640  // Internal buffer width
#define FRAMEBUFFER_HEIGHT 240 // Internal buffer height
#define DISPLAY_WIDTH 640      // Display width (2x scale)
#define DISPLAY_HEIGHT 480     // Display height (2x scale)

#define FRAMEBUFFER_PIXEL_WIDTH 2  // Pixel width in the framebuffer
#define FRAMEBUFFER_PIXEL_HEIGHT 1 // Pixel height in the framebuffer (1:2 vertical scale on display)

// Frame buffer structure
typedef struct
{
    volatile uint8_t current_display_buffer; // Index of the currently displayed buffer (0 or 1)
} framebuffer_t;

// FIFO message structure (packed into 32 bits)
typedef union
{
    struct
    {
        uint8_t buffer_index; // Index into the line data buffers array
    };
    uint32_t raw; // Raw 32-bit value for FIFO transfer
} fifo_msg_t;

// Initialize frame buffer
void framebuffer_init(void);

// Deinitialize frame buffer
void framebuffer_deinit(void);

// Get the current display buffer
const uint8_t *framebuffer_get_display(void);

// Get the current back buffer for drawing
uint8_t *framebuffer_get_draw(void);

// Commit the current back buffer to the display
void framebuffer_commit(void);

// Swap the front and back buffers
bool framebuffer_swap(void);

// Wait for the next framebuffer to be committed
bool framebuffer_wait_ready(void);

// Get a scaled scanline for DVI output
void framebuffer_get_line(uint8_t *line_buffer, uint32_t line);

// Get the address of the current display buffer
uint32_t framebuffer_get_addr(uint32_t line);

#endif // FRAME_BUFFER_H
