#ifndef _TRACKIDENTIFY_H
#define _TRACKIDENTIFY_H

#include "stdint.h"
#include "ImgProc.h"
#include "uart.h"
#include "root.h"
#include "TypeDef.h"

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
road_type_type GetRoadType(void);
void CurveCompensate(void);
void CrossRoadCompensate(void);
void CurveSlopeUpdate(int16_t row);
void HugeCurveDeal(int16_t row);
bool StraightRoadJudge(void);
void FittingMidLine( void );
void ImgRevise(int16_t row);

extern uint16_t LeftEdge[IMG_ROW];
extern uint16_t RightEdge[IMG_ROW];
extern uint16_t MidLine[IMG_ROW];
extern  bool LeftFlag,RightFlag;
extern int16_t rightBorderNotFoundCnt;
extern int16_t leftBorderNotFoundCnt;
extern mode MODE;
extern float leftSlope[IMG_ROW];
extern float leftZero[IMG_ROW];
extern float rightSlope[IMG_ROW];
extern float rightZero[IMG_ROW];
extern float middleSlope[IMG_ROW];
extern float middleZero[IMG_ROW];
extern uint16_t avgMidLine;

#endif

