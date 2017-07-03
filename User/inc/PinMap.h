#ifndef _PINMAP_H
#define _PINMAP_H

/* Motor */
#define LEFT_MOTOR_FORWARD_MAP      FTM0_CH0_PC01//PWM3ºÚ  ±³¶Ô¾µÍ·£¬ÓÒ±ßÊÇÓÒ
#define LEFT_MOTOR_REVERSE_MAP      FTM0_CH1_PC02//PWM4°×
#define RIGHT_MOTOR_REVERSE_MAP     FTM0_CH2_PC03//PWM2×Ï
#define RIGHT_MOTOR_FORWARD_MAP     FTM0_CH3_PC04//PWM1»Ò
#define MOTOR_PORT                  HW_FTM0
#define LEFT_MOTOR_FORWARD_CHL      HW_FTM_CH0
#define LEFT_MOTOR_REVERSE_CHL      HW_FTM_CH1
#define RIGHT_MOTOR_REVERSE_CHL     HW_FTM_CH2
#define RIGHT_MOTOR_FORWARD_CHL     HW_FTM_CH3

/* Encoder */    //1 GND  2 3.3-5V  3 A  4 B
#define RIGHT_ENCODER_MAP      FTM1_QD_PHA_PA12_PHB_PA13
#define LEFT_ENCODER_MAP       FTM2_QD_PHA_PB18_PHB_PB19//»Æ³È
#define RIGHT_ENCODER_CHL      HW_FTM1
#define LEFT_ENCODER_CHL       HW_FTM2

/* Gyro */
#define GYRO_MAP                ADC1_SE9_PB1//V2
#define DIR_GYRO_MAP            ADC1_SE8_PB0//V1

/* Acce */
#define ACCE_MMAX_MAP           ADC0_SE13_PB3 
#define ACCE_MMAZ_MAP           ADC0_SE12_PB2

//Data Communication
#define  DATACOMM_IMG_TRANS_MAP     UART4_RX_PE25_TX_PE24
#define  DATACOMM_IMG_TRANS_CHL     HW_UART4
#define  DATACOMM_IMG_TRANS_BAUD    115200
#define  DATACOMM_VISUALSCOPE_CHL   DATACOMM_IMG_TRANS_CHL
//#define  DATACOMM_DEBUG_MAP         UART4_RX_PE25_TX_PE24 // only a place-holder, depending on hardware
//#define  DATACOMM_DEBUG_CHL         HW_UART4
//#define  DATACOMM_DEBUG_BAUD        115200

//Camera
#define  CAMERA_HREF_PORT  HW_GPIOD
#define  CAMERA_HREF_PIN   0
#define  CAMERA_VSYN_PORT  HW_GPIOC
#define  CAMERA_VSYN_PIN   18
#define  CAMERA_DATA_PORT  HW_GPIOD
#define  CAMERA_DATA_PIN   1
#define  CAMERA_ODEV_PORT  HW_GPIOD
#define  CAMERA_ODEV_PIN   3
#define  CAMERA_DATA_READ  PDin(CAMERA_DATA_PIN)

//Mode Switch
#define DIP_PORT                HW_GPIOA
#define DIP1_PIN                17
#define DIP2_PIN                16
#define DIP3_PIN                15
#define DIP4_PIN                14

//Alert
#define  BUZZLE_PORT  HW_GPIOB
#define  BUZZLE_PIN   9
#define  BUZZLE_ON    (PBout(BUZZLE_PIN) = 0)
#define  BUZZLE_OFF   (PBout(BUZZLE_PIN) = 1)
#define  BUZZLE_REVERSE			(PBout(BUZZLE_PIN) = !PBout(BUZZLE_PIN))
//Infrared
#define  IR_PORT  HW_GPIOD
#define  IR_PIN   0
#define  IR_READ  PDin(IR_PIN)

#endif
