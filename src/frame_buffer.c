#include <string.h>
#include "frame_buffer.h"

// Double frame buffer storage
static uint8_t frame_buffers[2][FRAME_HEIGHT][FRAME_WIDTH];
static uint8_t current_display_buffer = 0;

// Buffer for expanded line (each pixel is doubled horizontally)
static uint8_t expanded_line[FRAME_WIDTH * 2];

void frame_buffer_init(void) {
    // Initialize both frame buffers with black color
    memset(frame_buffers, 0, sizeof(frame_buffers));
    current_display_buffer = 0;
}

const uint8_t* frame_buffer_get_display(void) {
    return (const uint8_t*)frame_buffers[current_display_buffer];
}

uint8_t* frame_buffer_get_draw(void) {
    return (uint8_t*)frame_buffers[1 - current_display_buffer];
}

// Get pointer to specific line in display buffer, expanded 2x horizontally
const uint8_t* frame_buffer_get_line(uint32_t line) {
    if (line >= FRAME_HEIGHT) {
        return NULL;
    }

    // Get the source line
    const uint8_t* src = frame_buffers[current_display_buffer][line / 2];

    // Expand pixels horizontally (only if this is a real line, not beyond buffer)
    if (line / 2 < FRAME_HEIGHT) {
        for (uint32_t x = 0; x < FRAME_WIDTH; x++) {
            expanded_line[x * 2] = src[x];
            expanded_line[x * 2 + 1] = src[x];
        }
        return expanded_line;
    }

    return NULL;
}

void frame_buffer_clear(uint8_t color) {
    // Clear the draw buffer
    memset(frame_buffers[1 - current_display_buffer], color, FRAME_WIDTH * FRAME_HEIGHT);
}

void frame_buffer_draw_pixel(uint32_t x, uint32_t y, uint8_t color) {
    if (x >= FRAME_WIDTH || y >= FRAME_HEIGHT) {
        return;
    }
    // Draw to the draw buffer
    frame_buffers[1 - current_display_buffer][y][x] = color;
}

void frame_buffer_swap(void) {
    // Swap display and draw buffers
    current_display_buffer = 1 - current_display_buffer;
}
