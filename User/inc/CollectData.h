#ifndef __COLLECTION_H
#define __COLLECTION_H

#define COLLECTDATA

#include "Root.h"

void Collection_Init(void);
void Sensor_Get(int32_t* leftSensor, int32_t* rightSensor);
int32_t MiddleSensor_Get(void);
int32_t Gyro_Get(void);
int32_t DirGyro_Get(void);
void Acce_Get(int32_t* mmax, int32_t* mmaz);

#endif
