#ifndef __PINMAP_H
#define __PINMAP_H

/* Motor */
#ifdef MOTOR
#define LEFT_MOTOR_FORWARD_MAP  FTM0_CH1_PA04
#define LEFT_MOTOR_REVERSE_MAP  FTM0_CH2_PA05
#define RIGHT_MOTOR_FORWARD_MAP FTM0_CH6_PA01
#define RIGHT_MOTOR_REVERSE_MAP FTM0_CH7_PA02
#define MOTOR_PORT              HW_FTM0
#define LEFT_MOTOR_FORWARD_CHL  HW_FTM_CH1
#define LEFT_MOTOR_REVERSE_CHL  HW_FTM_CH2
#define RIGHT_MOTOR_FORWARD_CHL HW_FTM_CH6
#define RIGHT_MOTOR_REVERSE_CHL HW_FTM_CH7
#endif

/* Encoder */
#ifdef ENCODER
#define LEFT_ENCODER_MAP        FTM2_QD_PHA_PB18_PHB_PB19
#define RIGHT_ENCODER_MAP       FTM1_QD_PHA_PA12_PHB_PA13
#define LEFT_ENCODER_CHL        HW_FTM2
#define RIGHT_ENCODER_CHL       HW_FTM1
#endif

///* Sensor */
//#define LEFT_SENSOR_MAP         ADC0_SE14_PC0
//#define RIGHT_SENSOR_MAP        ADC0_SE15_PC1

/* Gyro */
#ifdef COLLECTDATA
#define GYRO_MAP                ADC0_SE18_E25
#define DIR_GYRO_MAP            ADC0_SE17_E24

/* Acce */
#define ACCE_MMAX_MAP           ADC1_SE18
#define ACCE_MMAZ_MAP           ADC0_SE23
#endif

///* Uart */
//#define UART_BLUETOOTH_MAP      UART4_RX_PC14_TX_PC15
//#define UART_BLUETOOTH_CHL      HW_UART4
//#define UART_BLUETOOTH_BAUDRATE 9600

/* Dipswitch */
#define DIP_PORT                HW_GPIOB
#define DIP4_PIN                20
#define DIP3_PIN                21
#define DIP2_PIN                22
#define DIP1_PIN                23

///* IRemote */
//#define IR_PORT                 HW_GPIOD
//#define IR_PIN                  7
//#define IR_DATA                 PDin(7)

/* ReedSwitch */
#define RS_PORT                 HW_GPIOB
#define RS_PIN                  2

#endif
