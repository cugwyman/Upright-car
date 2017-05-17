#ifndef _PARAM_H
#define _PARAM_H

//Angle
#define AC_Set              256

#define Kalman_Sample_Time   0.0055
#define AC_PID_P             190.0 // 160.0
#define AC_PID_D             8 //   3.5
#define MMAX_ZERO            1932
#define MMAZ_ZERO            716
#define GYRO_ZERO_SAMPLE     1000
#define MMA_GAIN             0.073//0.232//    //0-90°,??????z-x?max?min=2000?,90/2000????
#define MMA_SCALE            3.178//*1.2//send to Kalman scale, end is 0.232

/* Speed */
#define VC_PID_P           210.0 //190.0// 
#define VC_PID_I           0.25//   0.25
#define VC_PID_D           90//40
//#define VC_Out_MAX         15000//25000
#define VC_PERIOD          4

/* Direction */
#define DC_PERIOD            2

/* Motor */
#define MOTOR_OUT_MAX     7000
#define FIX_OUT           0

//NVIC
#define  HREF_IRQ  PORTD_IRQn
#define  VSYN_IRQ  PORTC_IRQn
#define  TIMR_IRQ  PIT0_IRQn

//PIT
#define  PIT_CHL  HW_PIT_CH0
#define  PIT_PRD  5000

//Camera
#define  IMG_ABDN_ROW    47
#define  IMG_ROW_INTV    4
#define  IMG_READ_DELAY  105 //170 for 8 nops

//Image Proc Bit Map Relative
/* byte is 8 (2^3) bits */
#define  SHIFT  3
/* make the after-shifted bit no more than 1 << 0x07 */
#define  MASK   0x07

#define  IMG_ROW    50
#define  IMG_COL    225
#define  WHITE_VAL  0
#define  IMG_WHITE  0xfe
#define  IMG_BLACK  0x00
#define  IMG_EOF    0xff

//Image Proc Flag
#define  STRAIGHT_ROAD  0x0001
#define  CROSS_ROAD     0x0002
#define  CIRCLE         0x0003
#define  LEFTCURVE      0x0004
#define  RIGHTCURVE     0x0005

//Double Car Relative
#define  ULTRA_SONIC_TIMER_CHL  HW_PIT_CH1
#define  ULTRA_SONIC_TIMER_PRD  50000
#define  TIMER_INIT_COUNT       2349993uL
#define  AVG_DISTANCE_BETWEEN   100
#define  DIFF_DISTANCE_MAX      20
#define  OVER_TAKING            0x0001

#endif
