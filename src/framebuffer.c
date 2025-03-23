#include <stdlib.h>
#include <string.h>
#include "framebuffer.h"
#include "dvi.h"

// Frame buffer
uint8_t *framebuf = NULL;

// Initialize and allocate frame buffer
void framebuffer_init(void) {
    size_t size = DVI_H_ACTIVE * DVI_V_ACTIVE * sizeof(uint8_t);

    // Allocate memory
    framebuf = malloc(size);
    if (framebuf == NULL) {
        return;
    }

    // Initialize with black
    framebuffer_fill(COLOR_BLACK);
}

// Fill frame buffer with specified color
void framebuffer_fill(uint8_t color) {
    if (framebuf != NULL) {
        size_t size = DVI_H_ACTIVE * DVI_V_ACTIVE * sizeof(uint8_t);
        memset(framebuf, color, size);
    }
}

// Free frame buffer
void framebuffer_deinit(void) {
    if (framebuf != NULL) {
        free(framebuf);
        framebuf = NULL;
    }
}
