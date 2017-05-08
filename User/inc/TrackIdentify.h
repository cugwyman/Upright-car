#ifndef _TRACKIDENTIFY_H
#define _TRACKIDENTIFY_H

#include "stdint.h"
#include "ImgProc.h"
#include "uart.h"
#include "root.h"

void EdgeSearch( uint16_t n );
void GetMidLine( uint16_t n );
 uint16_t CurveCal(uint16_t n);
// uint16_t SlopeCal( uint16_t n);
// uint16_t BlockRec(uint16_t n);
// uint16_t CircleRec(uint16_t n);
void SensorGet(float* farimg, float* nearimg);
bool LeftBorderSearch(int16_t row);
bool LeftBorderSearchInRange(int16_t row, int16_t startIndex);
bool RightBorderSearch(int16_t row);
bool RightBorderSearchInRange(int16_t row, int16_t startIndex);
void MiddleLineUpdate(int16_t row);

extern uint16_t LeftEdge[OV7725_H];
extern uint16_t RightEdge[OV7725_H];
extern uint16_t MidLine[OV7725_H];
extern  bool LeftFlag,RightFlag;
extern mode MODE;

#endif

