#include "uart_normal.h"

void UartInit(void)
{
    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
}
/* CH Kinetis�̼��� V2.50 �汾 */
/* �޸���Ƶ ��ʹ�� CMSIS��׼�ļ� startup_MKxxxx.c �е� CLOCK_SETUP �� */

/* UART ���ٳ�ʼ���ṹ��֧�ֵ�����* ʹ��ʱ�����Ƽ���׼��ʼ�� */
/*
 UART1_RX_PE01_TX_PE00   
 UART0_RX_PF17_TX_PF18   
 UART3_RX_PE05_TX_PE04   
 UART5_RX_PF19_TX_PF20   
 UART5_RX_PE09_TX_PE08   
 UART2_RX_PE17_TX_PE16   
 UART4_RX_PE25_TX_PE24   
 UART0_RX_PA01_TX_PA02   
 UART0_RX_PA15_TX_PA14   
 UART3_RX_PB10_TX_PB11   
 UART0_RX_PB16_TX_PB17   
 UART1_RX_PC03_TX_PC04   
 UART4_RX_PC14_TX_PC15   
 UART3_RX_PC16_TX_PC17   
 UART2_RX_PD02_TX_PD03   
 UART0_RX_PD06_TX_PD07   
 UART2_RX_PF13_TX_PF14   
 UART5_RX_PD08_TX_PD09   
*/

 /*
 ʵ��Ч����ʹ�ÿ������ϵ�UART�ӿں͵���ͨ�ţ�
        ������ÿ��500ms�����ݷ��͵����ԵĴ��ڵ���������
        ���ڵ�ͨ���ٶ�Ϊ115200 8λ��Ч����λ
    ע�⣺��Ҫʹ��USB���������ӵ����Զ˺Ϳ������UART�ӿ�
         ���Զ���Ҫ��װCH340�����ʹ��ڵ�������
*/

//void Uart_Init(void)
//{
////    uint32_t instance; /*��� UART ��ģ��� */
////    DelayInit();
////    GPIO_QuickInit(HW_GPIOE, 6, kGPIO_Mode_OPP);
//    
//    /* ��ʼ��UART ʹ�ÿ��ٳ�ʼ����ʽ ������ 115200 ��������Ĭ�� ���س�ʼ���� UART��ģ��� */
//    UART_QuickInit(UART0_RX_PD06_TX_PD07, 115200);
//    
//    /* ����һ�ֳ�ʼ����ʽ ��׼��ʼ�� ��΢���� �����Ƽ� */
////    UART_InitTypeDef UART_InitStruct1 = {0};
////    UART_InitStruct1.instance = HW_UART0;     //����ʹ��UART0�˿�
////    UART_InitStruct1.baudrate = 115200;       //����ͨ���ٶ�Ϊ115200
////    UART_InitStruct1.parityMode = kUART_ParityEven; //����У��ģʽ
////    UART_InitStruct1.bitPerChar = kUART_8BitsPerChar; //�������ݴ���λ��
////    UART_Init(&UART_InitStruct1);
////    
////    /* ��ʼ��������ռ�õ����� ����kPinAlt3Ϊ���Ÿ���3ѡ�� ������оƬ Reference Manuel �ϵ� Signal Multiplexing and Signal Descriptions һ���в鵽*/
////    /* ��ʹ�ÿ��ٳ�ʼ��ʱ �������ڳ�ʼ������ ��Ϊ���ٳ�ʼ���Ѿ���ʼ������ */
////    PORT_PinMuxConfig(HW_GPIOD, 6, kPinAlt3);
////    PORT_PinMuxConfig(HW_GPIOD, 7, kPinAlt3);
//    /* ��ʹ�ô��ڳ�ʼ���� printf ��Ĭ�����ӵ���һ������ʼ���Ĵ�����*/
////    UART_printf(HW_UART0, "UART%d OK! Hello Kinetis\r\n");
//    
////    while(1)
////    {
////        /* ���� ���ֽڷ��� ���� ע�� HW_UART0�������Ѿ���ʼ������ģ�� ���� ����������*/
////        UART_WriteByte(HW_UART0, 'h');
////        UART_WriteByte(HW_UART0, 'e');
////        UART_WriteByte(HW_UART0, 'l');
////        UART_WriteByte(HW_UART0, 'l');
////        UART_WriteByte(HW_UART0, 'o');
////        UART_WriteByte(HW_UART0, '\r');
////        UART_WriteByte(HW_UART0, '\n');
////        /* ��˸С�� */
////        GPIO_ToggleBit(HW_GPIOE, 6);
////        DelayMs(500);
////    }
//}
