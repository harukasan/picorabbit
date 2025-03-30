#include <string.h>
#include <stdio.h>

#include "hardware_interface.h"
#include "framebuffer.h"
#include "draw.h"
#include "dvi.h"

void framebuffer_init(void) {
}

void framebuffer_deinit(void) {
    // Nothing to do
}

const uint8_t *framebuffer_get_display(void) {
    return dvi_get_frame_buffer();
}

uint8_t *framebuffer_get_draw(void) {
    return dvi_get_frame_buffer();
}

void framebuffer_commit(void) {
}

void frame_buffer_get_scaled_line(uint8_t* line_buffer, uint32_t line) {
}

// Wait for the next framebuffer to be committed
bool framebuffer_wait_ready(void) {
    return true;
}

// Swap the front and back buffers
bool framebuffer_swap(void) {
    return true;
}

