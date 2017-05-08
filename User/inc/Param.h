#ifndef __PARAM_H
#define __PARAM_H

/* ºê¿ª¹Ø*/
#define AC
#define VC
//#define DC
#define xOUT_JUDGE
#define xSINGLE_VC
#define DYNAMIC_DC_PID
#define xFIX
//#define RS_JUDGE
//#define SLOW_DOWN
#define x__DEBUG__


/* ½Ç¶È»· */
#define Kalman_Sample_Time   0.0055
#define AC_PID_P           190.0 // 160.0
#define AC_PID_D             4 //   3.5
#define MMAX_ZERO         1956
#define MMAZ_ZERO          716
#define GYRO_ZERO_SAMPLE  1000
#define MMA_GAIN          0.073//0.232//    //0-90°,??????z-x?max?min=2000?,90/2000????
#define MMA_SCALE         3.178//*1.2//send to Kalman scale, end is 0.232

/* ËÙ¶È»·*/
#define VC_PID_P           190.0 //190.0// 
#define VC_PID_I             0.15//   0.25
#define VC_PID_D             30
#define VC_Out_MAX       8000//25000
#define VC_PERIOD            4

/*·½Ïò»·*/
#define DC_PERIOD            2

/*½Ç¶ÈÉè¶¨Ä¿±ê*/
#define AC_Set              230//156//170zero,???

//Road Flag
#define  CIRCLE       1
#define  STOP         2    
#define  START        3

#define  IMG_BORDER_SCAN_COMPENSATION  20  //±ß½ç²¹³¥±äÁ¿
#define  WIDE_ROAD_SIZE                50

/* µç»ú²ÎÊý */
#define MOTOR_OUT_MAX     5000
#define FIX_OUT           0

/* ·¢³µÑÓÊ± */
#define BEGIN_DELAY       4000

//NVIC
#define  VSYN_IRQ  PORTC_IRQn
#define  TIMR_IRQ  PIT0_IRQn

//PIT
#define  PIT_CHL  HW_PIT_CH0
#define  PIT_PRD  5000

//camera
//0: 80x60
//1: 160x120
//2: 240x180
#define IMAGE_SIZE  0

#if (IMAGE_SIZE  ==  0)
#define OV7725_W    80
#define OV7725_H    60

#elif (IMAGE_SIZE == 1)
#define OV7725_W    160
#define OV7725_H    120

#elif (IMAGE_SIZE == 2)
#define OV7725_W    240
#define OV7725_H    180

#else
#error "Image Size Not Support!"
#endif

#define IMG_MIDPOINT 40

#endif
