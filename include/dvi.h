#ifndef DVI_H
#define DVI_H

#include "dvi_defines.h"

// DMA channel definitions
#define DMACH_PING 0
#define DMACH_PONG 1

// Start DVI output
void dvi_start();

// Wait for DMA transfer completion
void dvi_wait_for_transfer();

#endif // DVI_H
