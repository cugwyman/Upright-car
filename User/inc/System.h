#ifndef __SYSTEM_H
#define __SYSTEM_H

/* Include documents for user part */
#include "Root.h"

#include "Motor.h"
#include "Encoder.h"
//#include "ftmQD.h"
#include "uart_normal.h"
//#include "CollectData.h"
#include "Velocity.h"
#include "Angle.h"
#include "Direction.h"


#define ReedSwitch_Get() !GPIO_ReadBit(RS_PORT, RS_PIN)

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

