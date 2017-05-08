#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "Root.h"
#include "ov7725.h"
#include "i2c.h"
#include "ImgProc.h"
#include "Motor.h"
#include "Encoder.h"
#include "Velocity.h"
#include "Angle.h"
#include "Direction.h"
#include "VisualScope.h"
#include "Display.h"
#include "Uarts.h"

extern int32_t AC_Out;
extern int32_t VC_Out;
extern int32_t DC_Out;
extern int32_t Left_Out, Right_Out;
#if defined(SINGLE_VC) || defined(VC) || defined(DC)
extern int32_t speed;
extern float distance;
#endif

void GeneralInit(void);

#endif
