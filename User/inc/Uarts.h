#ifndef __UARTS_H
#define __UARTS_H

#define UARTS

#include "uart.h"
#include "root.h"
#include "TrackIdentify.h"
extern mode MODE;

void UartInit(void);
void ImgTrans(uint8_t imgBuf[IMG_ROW][IMG_COL]);
void ImgTrans2(uint8_t imgBuf[IMG_ROW][IMG_COL]);
void ImgTrans3(uint8_t imgBuf[IMG_ROW][IMG_COL]);
void ImgTrans4(uint8_t imgBuf[IMG_ROW][IMG_COL]);

#endif


