#ifndef __FTMQD_H
#define __FTMQD_H

#include "gpio.h"
#include "common.h"
#include "uart.h"
#include "ftm.h"
#include "pit.h"
#include "adc.h"
#include "Motor.h"

void FtmQD_Init(void);
void PIT_ISR(void);
void _PIT_Init(void);

#endif


