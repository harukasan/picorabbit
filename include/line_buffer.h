#ifndef LINE_BUFFER_H
#define LINE_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include "dvi_defines.h"

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

// Line buffer structure
typedef struct {
    uint8_t *buffer;                  // Double buffer for line data [2 * line_width]
    uint32_t line_width;              // Width of each line in pixels
    volatile uint32_t current_buffer; // Current buffer index (0 or 1)
} line_buffer_t;

// FIFO message structure (packed into 32 bits)
typedef union {
    struct {
        uint16_t line_y;       // Line number in the frame (0-479)
        uint16_t buffer_index; // Index into the line data buffers array
    };
    uint32_t raw;              // Raw 32-bit value for FIFO transfer
} fifo_msg_t;

// Initialize line buffer
void line_buffer_init(uint32_t width);

// Deinitialize line buffer
void line_buffer_deinit(void);

// Wait for line buffer to be ready (called by Core 0)
bool line_buffer_wait_ready(void);

// Get the back buffer
uint8_t* line_buffer_get_back_buffer(void);

// Get the front buffer
uint8_t* line_buffer_get_front_buffer(void);

// Commit a line to the buffer
void line_buffer_commit_line(uint16_t line_y);

// Get the drawing line (Called by Core 1)
const uint8_t* line_buffer_get_line(uint32_t line_number);

// Swap the buffers (Called by Core 1)
void line_buffer_swap(void);

#endif // LINE_BUFFER_H
