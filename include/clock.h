#ifndef CLOCK_H
#define CLOCK_H

// Initialize system and peripheral clocks
void init_clock(void);

// Configure HSTX clock using USB PLL
void set_hstx_clock(void);

#endif // CLOCK_H
