#ifndef __ANGLE_H
#define __ANGLE_H

#include "Root.h"
#include "CollectData.h"


#define Q_angle     0.001 //Ω«∂»‘Î…˘
#define Q_gyro      0.0005//∆Ø“∆‘Î…˘
#define R_angle     0.05  //Ω«∂»≤‚¡ø‘Î…˘÷µ
#define C_0         1

extern float Kal_Gyro;
extern float gyro;
extern int32_t mmax, mmaz;
extern float angle, angleSpeed;
extern int16_t Angle_Kalman;
extern int16_t Pre_Angle_Kalman;
extern float angleSpeedIntegral;

int32_t AngleProc(void);

#endif
