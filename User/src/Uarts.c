#include "Uarts.h"

void UartInit(void) {
     UART_QuickInit(UART4_RX_PE25_TX_PE24, 115200);
//			printf("aaaa");
}

void uart_putbuff (uint8_t *buff, uint32_t len)
{
    while(len--)
    {
			  UART_WriteByte(HW_UART4,0x00|(*buff));
        buff++;
    }
}

void vcan_sendimg(uint8_t *imgaddr, uint32_t imgsize)
{
#define CMD_IMG     1
    uint8_t cmdf[2] = {0x01, 0xfe};    
    uint8_t cmdr[2] = {0xfe, 0x01};   
    uart_putbuff(cmdf, sizeof(cmdf));    

    uart_putbuff((uint8_t *)imgaddr, imgsize); 

    uart_putbuff(cmdr, sizeof(cmdr));
}

void ImgTrans(void)
{
	uint16_t i,j;
	uint8_t proimgBuf[OV7725_H][OV7725_W];
	for(i=0;i<OV7725_H;i++)
	{
		for(j=0;j<OV7725_W;j++)
			{
				if((j==LeftEdge[i])||(j==RightEdge[i])||(j==MidLine[i]))
					proimgBuf[i][j] = 0;
				else proimgBuf[i][j] = imgBuf[i][j];
			}
	}
	vcan_sendimg((uint8_t *)proimgBuf,sizeof (proimgBuf));
}
