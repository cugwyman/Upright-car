#ifndef _IMGPROC_H
#define _IMGPROC_H

#include "Root.h"
#include "Uarts.h"
#include "dma.h"
#include "TrackIdentify.h"

void ImgProcInit(void);

extern uint8_t imgBuf[OV7725_H][OV7725_W];
extern uint16_t imgProcFlag;

#endif
