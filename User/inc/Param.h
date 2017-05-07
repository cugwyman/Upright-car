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
//#define RS_JUDGE
//#define SLOW_DOWN
#define x__DEBUG__

///* 传感器采样滤波 */
//#define SENSOR_FLTR_SAMPLE  10

/* 角度环 */
#define Kalman_Sample_Time   0.0055
#define AC_PID_P           200.0 // 160.0
#define AC_PID_D             4.5 //   3.5
#define MMAX_ZERO         1956
#define MMAZ_ZERO          716
#define GYRO_ZERO_SAMPLE  1000
#define MMA_GAIN          0.073//0.232//    //0-90°，即正反车测值z-x的max减min=2000多，90/2000多得比例
#define MMA_SCALE         3.178//*1.2//send to Kalman scale, end is 0.232

/* 速度环 */
#define VC_PID_P           200.0 //190.0// 
#define VC_PID_I             0.15//   0.25
#define VC_PID_D             35
#define VC_Out_MAX       8000//25000
#define VC_PERIOD            4
//#define MODE.VC_Set           50//27//21// 18//  10//40设定，速度为30
//#define VC_MAX           52
//#define VC_MIN           48

/* 方向环 */
//#ifdef DYNAMIC_DC_PID               //按MODE.VC_Set
//#define MODE.DC_PID_P_COEF      70 //3// 100 // 120 //70 //90  //            470.0
//#define MODE.DC_P_MIN           800//2200// 1000//500//700 //            3200
//#define MODE.DC_P_MAX           2200//3300// 1314//800//1212//            50000
//#else  /* xDYNAMIC_DC_PID */
//#define DC_PID_P         1314
//#endif /* DYNAMIC_DC_PID */

//#define MODE.DC_PID_D           2.8//3// 1.6//1.3// 8.0 // 10.5
//#define MODE.DC_Out_MAX       2500//4000//
#define DC_PERIOD            2

#define DC_OUT_COEF_LEFT   1
#define DC_OUT_COEF_RIGHT  1

//#define dirAngleSpeedMax    300
//#define dirAngleSpeedMin    -300

#define OUT_JUDGE_STD       50
#define TURN_FLAG	       200

/* 角度和速度设定目标 */
#define AC_Set              260//156//170zero,小往后
//#define VSET_GEAR4          40
//#define VSET_GEAR3          37
//#define VSET_GEAR2          34
////#define VSET_GEAR1          38
//#define VSET_GEAR0           0

//#define  foresight4       30//23//17//15
//#define  foresight3       28//23//17//15
//#define  foresight2       26//23//17//15

//Camera
#define  IMG_ROW        50
#define  IMG_COL        225
#define  IMG_ABDN_ROW   19
#define  IMG_ROW_INTV   5

#define  IMG_WHITE      0xfe
#define  IMG_BLACK      0x00
#define  IMG_FRAME_FIN  0xff

#define  IMG_MIDPOINT    112

//Road Flag
#define  CIRCLE       1
#define  STOP         2    
#define  START        3
#define  STRAIGHTROAD 4
#define  CURVEROAD    5

#define  IMG_BORDER_SCAN_COMPENSATION  30  //边界补偿变量
#define  WIDE_ROAD_SIZE                80


/* 电机参数 */
#define MOTOR_OUT_MAX     5000
#define FIX_OUT           0

/* 发车延时 */
#define BEGIN_DELAY       4000

//NVIC
#define  HREF_IRQ  PORTD_IRQn
#define  VSYN_IRQ  PORTC_IRQn
#define  TIMR_IRQ  PIT0_IRQn

//PIT
#define  PIT_CHL  HW_PIT_CH0
#define  PIT_PRD  5000


#endif
