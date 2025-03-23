#ifndef DVI_DEFINES_H
#define DVI_DEFINES_H

// DVI timing constants for 480p60 (640x480@60Hz)
// Reference: CEA-861-D specification
#define DVI_H_ACTIVE        640
#define DVI_H_FRONT_PORCH   16
#define DVI_H_SYNC          96
#define DVI_H_BACK_PORCH    48
#define DVI_H_TOTAL         (DVI_H_ACTIVE + DVI_H_FRONT_PORCH + DVI_H_SYNC + DVI_H_BACK_PORCH)

#define DVI_V_ACTIVE        480
#define DVI_V_FRONT_PORCH   10
#define DVI_V_SYNC          2
#define DVI_V_BACK_PORCH    33
#define DVI_V_TOTAL         (DVI_V_ACTIVE + DVI_V_FRONT_PORCH + DVI_V_SYNC + DVI_V_BACK_PORCH)

#define DVI_REFRESH_RATE    60

// TMDS clock calculation
// HSTX clock = H_total * V_total * refresh_rate * 10 / 2
#define DVI_HSTX_CLOCK_HZ   ((DVI_H_TOTAL * DVI_V_TOTAL * DVI_REFRESH_RATE * 10ULL) / 2)

#endif // DVI_DEFINES_H
