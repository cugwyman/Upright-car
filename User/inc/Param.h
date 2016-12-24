#ifndef __PARAM_H
#define __PARAM_H

/* 宏开关 */
#define AC
#define VC
#define DC
#define xOUT_JUDGE
#define xSINGLE_VC
#define DYNAMIC_DC_PID
#define xFIX
#define RS_JUDGE
#define xSLOW_DOWN
#define x__DEBUG__

/* 传感器采样滤波 */
#define SENSOR_FLTR_SAMPLE  10

/* 角度环 */
#define Kalman_Sample_Time   0.0055
#define AC_PID_P           190.0 // 160.0
#define AC_PID_D             5.5 //   3.5
#define MMAX_ZERO         1939
#define MMAZ_ZERO          780
#define GYRO_ZERO_SAMPLE  1000
#define MMA_GAIN             0.232

/* 速度环 */
#define VC_PID_P           200.0 // 220.0
#define VC_PID_I             0.2 //   0.25
#define VC_PID_D            50.0
#define VC_Out_MAX       25000
#define VC_PERIOD            4

/* 方向环 */
#ifdef DYNAMIC_DC_PID
#define DC_PID_P_COEF      470.0
#define DC_P_MIN          3200
#define DC_P_MAX         50000
#else  /* xDYNAMIC_DC_PID */
#define DC_PID_P         20000
#endif /* DYNAMIC_DC_PID */

#define DC_PID_D             8.0 // 10.5
#define DC_PERIOD            2

#define OUT_JUDGE_STD       50
#define TURN_FLAG	       300

/* 角度和速度设定目标 */
#define AC_Set              87
#define VSET_GEAR4          90
#define VSET_GEAR3          80
#define VSET_GEAR2          70
#define VSET_GEAR1          60
#define VSET_GEAR0           0

/* 电机参数 */
#define MOTOR_OUT_MAX     9000
#define FIX_OUT           4000

/* 发车延时 */
#define BEGIN_DELAY       4000

#endif
