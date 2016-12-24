#ifndef __MOTOR_H
#define __MOTOR_H

#define MOTOR
#define  MOTOR_STOP  MotorOut(0, 0)

#include "Root.h"

void MotorInit(void);
void MotorOut(int16_t LEFT_MOTOR_OUT, int16_t RIGHT_MOTOR_OUT);


#endif


