#ifndef LINEBUFFER_H
#define LINEBUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include "dvi_defines.h"

// Color definitions (RGB332)
#define COLOR_BLACK 0x00
#define COLOR_WHITE 0xFF

// Line buffer structure
typedef struct {
    uint8_t *buffer;              // Double buffer for line data [2 * line_width]
    uint32_t line_width;          // Width of each line in pixels
    volatile uint32_t current_buffer; // Current buffer index (0 or 1)
    volatile uint32_t current_line;   // Current line being processed
} line_buffer_t;

// FIFO message structure (packed into 32 bits)
typedef union {
    struct {
        uint16_t line_number;    // Line number in the frame (0-479)
        uint16_t buffer_index;   // Index into the line data buffers array
    };
    uint32_t raw;               // Raw 32-bit value for FIFO transfer
} fifo_msg_t;

// Function prototypes
void linebuffer_init(uint32_t width);
void linebuffer_deinit(void);
uint32_t linebuffer_current_line(void);
bool linebuffer_add_line(const uint8_t *data);
bool linebuffer_write_line(const uint8_t *data, uint32_t line_number);
const uint8_t* linebuffer_get_line(uint32_t line_number);
void linebuffer_swap(void);
bool linebuffer_wait_ready(void);

#endif // LINEBUFFER_H
