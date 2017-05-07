#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#include "common.h"


typedef struct PID_speed_mode {
    int16_t DC_PID_P_COEF;
    int16_t DC_P_MIN;
    int16_t DC_P_MAX;
    float DC_PID_D;
    int16_t DC_Out_MAX;
    
    int16_t VC_Set;
    int16_t foresight;
}
mode;


#endif
