#include <stdlib.h>
#include <string.h>
#include "line_buffer.h"
#include "pico/multicore.h"

#define CORE_READY_FLAG 123

// Global line buffer instance
static line_buffer_t line_buffer;

// Line data buffers for double buffering
static uint8_t *line_data_buffers = NULL;
static uint32_t line_width = 0;

// Initialize line buffer and FIFO
void line_buffer_init(uint32_t width) {
    line_width = width;

    // Allocate double buffer
    line_buffer.buffer = (uint8_t *)malloc(2 * width);
    if (line_buffer.buffer == NULL) {
        return;
    }
    memset(line_buffer.buffer, 0, 2 * width);
    line_buffer.line_width = width;
    line_buffer.current_buffer = 0;

    // Signal that we're ready if we're on Core 1
    if (get_core_num() == 1) {
        multicore_fifo_push_blocking(CORE_READY_FLAG);
    }
}

// Deinitialize line buffer
void line_buffer_deinit(void) {
    if (line_buffer.buffer) {
        free(line_buffer.buffer);
        line_buffer.buffer = NULL;
    }
}

// Wait for line buffer to be ready (called by Core 0)
bool line_buffer_wait_ready(void) {
    if (get_core_num() != 0) {
        return false;
    }

    uint32_t flag = multicore_fifo_pop_blocking();
    return flag == CORE_READY_FLAG;
}

// Get the back buffer
uint8_t* line_buffer_get_back_buffer(void) {
    return line_buffer.buffer + line_buffer.current_buffer * line_width;
}

// Get the front buffer
uint8_t* line_buffer_get_front_buffer(void) {
    return line_buffer.buffer + !line_buffer.current_buffer * line_width;
}

// Commit a line to the buffer
void line_buffer_commit_line(uint16_t line_y) {
    if (line_y >= DVI_V_ACTIVE || !line_buffer.buffer) {
        return;
    }
    // Notify Core 1 that new line is ready
    if (get_core_num() == 0) {
        fifo_msg_t msg = {
            .line_y = line_y,
            .buffer_index = !line_buffer.current_buffer // back buffer
        };
        multicore_fifo_push_blocking(msg.raw);
    }
}

// Get line data for DVI output (Called by Core 1)
const uint8_t* line_buffer_get_line(uint32_t line_number) {
    // Check if there is new data in the FIFO
    if (multicore_fifo_rvalid()) {
        uint32_t raw = multicore_fifo_pop_blocking();
        fifo_msg_t msg = { .raw = raw };

        if (msg.line_y == line_number) {
            // Return the current buffer for DMA
            return line_buffer.buffer + msg.buffer_index * line_width;
        }
    }
    // Return current buffer if no new data
    return line_buffer.buffer + line_buffer.current_buffer * line_width;
}

// Swap line buffers
void line_buffer_swap(void) {
    line_buffer.current_buffer = !line_buffer.current_buffer;
}

