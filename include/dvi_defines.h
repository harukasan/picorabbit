#ifndef DVI_DEFINES_H
#define DVI_DEFINES_H

// Include resolution specific timing constants
#include "dvi_timing_1280x720p60.h"

// Total timing values
#define DVI_H_TOTAL (DVI_H_ACTIVE + DVI_H_FRONT_PORCH + DVI_H_SYNC + DVI_H_BACK_PORCH)
#define DVI_V_TOTAL (DVI_V_ACTIVE + DVI_V_FRONT_PORCH + DVI_V_SYNC + DVI_V_BACK_PORCH)

// TMDS clock calculation
#define DVI_HSTX_CLOCK_HZ   ((DVI_H_TOTAL * DVI_V_TOTAL * DVI_REFRESH_RATE * 10ULL) / 2)

#endif // DVI_DEFINES_H
