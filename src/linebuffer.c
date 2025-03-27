#include <stdlib.h>
#include <string.h>
#include "linebuffer.h"
#include "pico/multicore.h"

#define CORE_READY_FLAG 123

// Global line buffer instance
static line_buffer_t line_buffer;

// Line data buffers for double buffering
static uint8_t *line_data_buffers = NULL;
static uint32_t line_width = 0;

// Initialize line buffer and FIFO
void linebuffer_init(uint32_t width) {
    line_width = width;

    // Allocate double buffer
    line_buffer.buffer = (uint8_t *)malloc(2 * width);
    if (line_buffer.buffer == NULL) {
        return;
    }
    memset(line_buffer.buffer, 0, 2 * width);
    line_buffer.line_width = width;
    line_buffer.current_buffer = 0;
    line_buffer.current_line = 0;

    // Signal that we're ready if we're on Core 1
    if (get_core_num() == 1) {
        multicore_fifo_push_blocking(CORE_READY_FLAG);
    }
}

// Free resources
void linebuffer_deinit(void) {
    if (line_buffer.buffer) {
        free(line_buffer.buffer);
        line_buffer.buffer = NULL;
    }
}

// Return current line number
uint32_t linebuffer_current_line(void) {
    return line_buffer.current_line;
}

// Add line data to current line of the buffer
bool linebuffer_add_line(const uint8_t *data) {
    linebuffer_write_line(data, line_buffer.current_line);
    line_buffer.current_line++;
    if (line_buffer.current_line >= DVI_V_ACTIVE) {
        line_buffer.current_line = 0;
    }
    return true;
}

// Write line data to buffer
bool linebuffer_write_line(const uint8_t *data, uint32_t line_number) {
    if (line_number >= DVI_V_ACTIVE || !line_buffer.buffer) {
        return false;
    }

    // Write to the back buffer
    uint back_buffer = !line_buffer.current_buffer;
    memcpy(line_buffer.buffer + back_buffer * line_width, data, line_width);

    // Notify Core 1 that new line is ready
    if (get_core_num() == 0) {
        fifo_msg_t msg = {
            .line_number = line_number,
            .buffer_index = back_buffer
        };
        multicore_fifo_push_blocking(msg.raw);
    }

    return true;
}

// Get line data for DVI output
const uint8_t* linebuffer_get_line(uint32_t line_number) {
    if (get_core_num() == 1) {
        // Core 1: Wait for line data from Core 0
        if (multicore_fifo_rvalid()) {
            uint32_t raw = multicore_fifo_pop_blocking();
            fifo_msg_t msg = { .raw = raw };

            // if (msg.line_number == line_number) {
                // Return the current buffer for DMA
                return line_buffer.buffer + line_buffer.current_buffer * line_width;
            //}
        }
    }

    // Return current buffer if no new data
    return line_buffer.buffer + line_buffer.current_buffer * line_width;
}

// Swap line buffers
void linebuffer_swap(void) {
    line_buffer.current_buffer = !line_buffer.current_buffer;
}

// Wait for Core 1 to be ready (called by Core 0)
bool linebuffer_wait_ready(void) {
    if (get_core_num() != 0) {
        return false;
    }

    uint32_t flag = multicore_fifo_pop_blocking();
    return flag == CORE_READY_FLAG;
}
