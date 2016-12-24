#ifndef __DIRECTION_H
#define __DIRECTION_H

#include "Root.h"
#include "CollectData.h"

extern float state;//传感器融合
extern int32_t leftSensor, rightSensor, dirAngleSpeed;
#if defined(OUT_JUDGE) || defined(RS_JUDGE)
extern bool out;
#endif
#ifdef DYNAMIC_DC_PID
extern float DC_PID_P;
#endif

int32_t Direction_Process(int32_t speed);

#endif
