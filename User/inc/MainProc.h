#ifndef _MAINPROC_H
#define _MAINPROC_H

#include "root.h"

#include "Motor.h"
#include "Encoder.h"
#include "DataComm.h"
#include "ImgProc.h"
#include "DirectionControl.h"
//#include "ModeSwitch.h"

#include "CollectData.h"
#include "Angle.h"
#include "Velocity.h"
#include "Direction.h"
#include "OLED.h"

extern int32_t AC_Out;
extern int32_t VC_Out;
extern int32_t DC_Out;
extern int32_t Left_Out, Right_Out;
#if defined(SINGLE_VC) || defined(VC) || defined(DC)
extern int32_t speed;
extern float distance;
#endif

void MainInit(void);

#endif