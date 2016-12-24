#ifndef __VELOCITY_H
#define __VELOCITY_H

#include "Root.h"


extern int VC_Max;
extern int VC_Min;
extern int VC_Set;

void Gear_Get(void);
int32_t Velocity_PID(int32_t set, int32_t nextpoint);
int32_t Velocity_Process(int32_t speed);

#endif
