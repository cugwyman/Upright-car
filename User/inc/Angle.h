#ifndef __ANGLE_H
#define __ANGLE_H

#include "Root.h"
#include "CollectData.h"


#define Q_angle     0.001
#define Q_gyro      0.0005
#define R_angle     0.05
#define C_0         1

extern float Kal_Gyro;
extern int32_t gyro;
extern int32_t mmax, mmaz;
extern int32_t angle, angleSpeed;
extern float Angle_Kalman;
extern float angleSpeedIntegral;

int32_t Angle_Process(void);

#endif
