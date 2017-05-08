#ifndef _IMGPROC_H
#define _IMGPROC_H

#define IMGPROC

#include "root.h"

void ImgProcInit(void);

extern uint8_t imgBuf[IMG_ROW][IMG_COL];
extern uint16_t imgProcFlag;
extern uint16_t StopFlagAnalyze;
extern uint16_t StopFlag;

extern float time;

#endif

