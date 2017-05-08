#ifndef __PINMAP_H
#define __PINMAP_H

/* Motor */
//#ifdef MOTOR
#define LEFT_MOTOR_FORWARD_MAP  FTM0_CH1_PC02//PWM3ºÚ  ±³¶Ô¾µÍ·£¬ÓÒ±ßÊÇÓÒ
#define LEFT_MOTOR_REVERSE_MAP  FTM0_CH0_PC01//PWM4°×
#define RIGHT_MOTOR_FORWARD_MAP FTM0_CH2_PC03//PWM2×Ï
#define RIGHT_MOTOR_REVERSE_MAP FTM0_CH3_PC04//PWM1»Ò
#define MOTOR_PORT              HW_FTM0
#define LEFT_MOTOR_FORWARD_CHL  HW_FTM_CH1
#define LEFT_MOTOR_REVERSE_CHL  HW_FTM_CH0
#define RIGHT_MOTOR_FORWARD_CHL HW_FTM_CH2
#define RIGHT_MOTOR_REVERSE_CHL HW_FTM_CH3
//#endif

/* Encoder */    //1 GND  2 3.3-5V  3 A  4 B
//#ifdef ENCODER
#define RIGHT_ENCODER_MAP      FTM1_QD_PHA_PA12_PHB_PA13
#define LEFT_ENCODER_MAP       FTM2_QD_PHA_PB18_PHB_PB19//»Æ³È
#define RIGHT_ENCODER_CHL      HW_FTM1
#define LEFT_ENCODER_CHL       HW_FTM2
//#endif


/* Gyro */
//#ifdef COLLECTDATA
#define GYRO_MAP                ADC0_SE13_PB3//V2
#define DIR_GYRO_MAP            ADC0_SE12_PB2//V1

/* Acce */
#define ACCE_MMAX_MAP           ADC1_SE8_PB0 
#define ACCE_MMAZ_MAP           ADC1_SE9_PB1
//#endif

/* Dipswitch */
#ifdef DIPSWITCH
#define DIP_PORT                HW_GPIOC
#define DIP1_PIN                9
#define DIP2_PIN                8
#define DIP3_PIN                7
#define DIP4_PIN                6
#endif

/* LEDSwitch */
#ifdef LEDSWITCH
#define LED_PORT                HW_GPIOA
#define LED1_PIN                15
#define LED2_PIN                16
#endif

//camera
#define CAMERA_I2C		I2C1_SCL_PE01_SDA_PE00

#define BOARD_OV7725_PCLK_PORT      HW_GPIOC
#define BOARD_OV7725_PCLK_PIN       (17)
#define BOARD_OV7725_VSYNC_PORT     HW_GPIOC
#define BOARD_OV7725_VSYNC_PIN      (18)
#define BOARD_OV7725_HREF_PORT      HW_GPIOA
#define BOARD_OV7725_HREF_PIN       (17)

/* Buzzer */
#ifdef BUZZER
#define BUZ_PORT                HW_GPIOB
#define BUZ_PIN                 21
#endif

/* IR */
#ifdef IR
#define BUZ_PORT                HW_GPIOE
#define BUZ_PIN                 24
#endif

#endif
