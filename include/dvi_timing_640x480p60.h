#ifndef DVI_TIMING_640x480P_H
#define DVI_TIMING_640x480P_H

// DVI timing constants for 480p60 (640x480@60Hz)
// Reference: CEA-861
// https://tomverbeure.github.io/video_timings_calculator

#define DVI_H_ACTIVE        640
#define DVI_H_FRONT_PORCH   16
#define DVI_H_SYNC          96
#define DVI_H_BACK_PORCH    48

#define DVI_V_ACTIVE        480
#define DVI_V_FRONT_PORCH   10
#define DVI_V_SYNC          2
#define DVI_V_BACK_PORCH    33

#define DVI_REFRESH_RATE    60

#endif // DVI_TIMING_640x480P_H
