#include <string.h>
#include "hardware_interface.h"

#include "framebuffer.h"

#define CORE_READY_FLAG 123

static framebuffer_t framebuffer;

// Buffer for expanded line (each pixel is doubled horizontally)
static uint8_t expanded_line[OUTPUT_WIDTH];

void framebuffer_init(void) {
    memset(&framebuffer, 0, sizeof(framebuffer));
    framebuffer.current_display_buffer = 0;
}

void framebuffer_deinit(void) {
    // Nothing to do
}

const uint8_t *framebuffer_get_display(void) {
    return (uint8_t *)&framebuffer.buffers[framebuffer.current_display_buffer];
}

uint8_t *framebuffer_get_draw(void) {
    return (uint8_t *)&framebuffer.buffers[!framebuffer.current_display_buffer];
}

void framebuffer_commit(void) {
    fifo_msg_t msg = {
        .buffer_index = !framebuffer.current_display_buffer // back buffer
    };
    multicore_fifo_push_blocking(msg.raw);
}

// Get pointer to specific line in display buffer, expanded 2x horizontally
const uint8_t* frame_buffer_get_line(uint32_t line) {
    if (line >= FRAMEBUFFER_HEIGHT) {
        return NULL;
    }

    // Get the source line
    const uint8_t* src = framebuffer.buffers[framebuffer.current_display_buffer];

    // Expand pixels horizontally (only if this is a real line, not beyond buffer)
    if (line / 2 < FRAMEBUFFER_HEIGHT) {
        for (uint32_t x = 0; x < FRAMEBUFFER_WIDTH; x++) {
            expanded_line[x * 2] = src[(line / 2) * FRAMEBUFFER_WIDTH + x];
            expanded_line[x * 2 + 1] = src[(line / 2) * FRAMEBUFFER_WIDTH + x];
        }
        return expanded_line;
    }

    return NULL;
}

// Wait for the next framebuffer to be committed
bool framebuffer_wait_ready(void) {
    if (get_core_num() != 0) {
        return false;
    }

    uint32_t flag = multicore_fifo_pop_blocking();
    return flag == CORE_READY_FLAG;
}

// Swap the front and back buffers
void framebuffer_swap(void) {
    // Check if there is new data in the FIFO
    if (multicore_fifo_rvalid()) {
        uint32_t raw = multicore_fifo_pop_blocking();
        fifo_msg_t msg = { .raw = raw };

        if (msg.buffer_index != framebuffer.current_display_buffer) {
            framebuffer.current_display_buffer = msg.buffer_index;
        }
    }
}

const uint8_t *framebuffer_get_scaled_line(uint32_t line_number) {
    if (line_number >= OUTPUT_HEIGHT) return NULL;

    // Calculate source line in the internal buffer
    uint32_t src_line = line_number / 2;
    if (src_line >= FRAMEBUFFER_HEIGHT) return NULL;

    // Get pointer to source line
    const uint8_t *src = &framebuffer.buffers[framebuffer.current_display_buffer][src_line * FRAMEBUFFER_WIDTH];

    // Scale horizontally (2x)
    for (uint32_t x = 0; x < FRAMEBUFFER_WIDTH; x++) {
        expanded_line[x * 2] = src[x];
        expanded_line[x * 2 + 1] = src[x];
    }

    return expanded_line;
}