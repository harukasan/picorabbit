#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include "dvi_defines.h"

// Color definitions (RGB332)
#define COLOR_BLACK 0x00
#define COLOR_WHITE 0xFF

// Frame buffer
extern uint8_t *framebuf;

// Function prototypes
void framebuffer_init(void);
void framebuffer_fill(uint8_t color);
void framebuffer_deinit(void);

#endif // FRAMEBUFFER_H
