#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#include "dvi_defines.h"

// Display resolution configuration
#define FRAMEBUFFER_WIDTH DVI_H_ACTIVE / 2  // Internal buffer width
#define FRAMEBUFFER_HEIGHT DVI_V_ACTIVE / 2 // Internal buffer height
#define OUTPUT_WIDTH DVI_H_ACTIVE           // DVI output width (2x scale)
#define OUTPUT_HEIGHT DVI_V_ACTIVE          // DVI output height (2x scale)

// Frame buffer structure
typedef struct {
    uint8_t buffers[2][FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT]; // Double buffer array [2][360*240]
    volatile uint8_t current_display_buffer;                     // Index of the currently displayed buffer (0 or 1)
} framebuffer_t;

// FIFO message structure (packed into 32 bits)
typedef union {
    struct {
        uint16_t buffer_index; // Index into the line data buffers array
    };
    uint32_t raw;              // Raw 32-bit value for FIFO transfer
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
void framebuffer_swap(void);

// Wait for the next framebuffer to be committed
bool framebuffer_wait_ready(void);

// Get a scaled scanline for DVI output
const uint8_t *framebuffer_get_scaled_line(uint32_t line_number);

#endif // FRAME_BUFFER_H