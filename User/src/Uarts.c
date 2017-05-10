#include "Uarts.h"
//mode MODE;

void UartInit(void) //串口初始化
{
    UART_QuickInit(UART0_RX_PB16_TX_PB17, 115200);
}
//*****************************************************************************/
//  发原图
//*****************************************************************************/
void ImgTrans(uint8_t imgBuf[IMG_ROW][IMG_COL])
{
    int16_t i, j;
	for(i = IMG_ROW - 1; i >= 0; i--)
	{
		for(j = 0; j < IMG_COL; j++)
		{
			if(imgBuf[i][j])
				UART_WriteByte(DATACOMM_IMG_TRANS_CHL, IMG_BLACK);
			else
				UART_WriteByte(DATACOMM_IMG_TRANS_CHL, IMG_WHITE);
		}
	}
	UART_WriteByte(DATACOMM_IMG_TRANS_CHL, IMG_FRAME_FIN);
}
//*****************************************************************************/
//  发中线图
//*****************************************************************************/
void ImgTrans2(uint8_t imgBuf[IMG_ROW][IMG_COL])//发中线图
{
	int i,j;
	for(i=IMG_ROW-1;i>=0;i--)
	{
		for(j=0;j<=IMG_COL-1;j++)
		{
			if((j==LeftEdge[i])||(j==RightEdge[i])||(j==MidLine[i]))
				UART_WriteByte(DATACOMM_IMG_TRANS_CHL,0x00);
			else 
					{
						if(imgBuf[i][j]==0x00)	
							UART_WriteByte(HW_UART0,IMG_WHITE);
						else
							UART_WriteByte(HW_UART0,IMG_BLACK);
					}
		}						
	}
	UART_WriteByte(DATACOMM_IMG_TRANS_CHL,IMG_FRAME_FIN);
}
//*****************************************************************************/
//  发前瞻图
//*****************************************************************************/
void ImgTrans3(uint8_t imgBuf[IMG_ROW][IMG_COL])//发前瞻图
{
	int i,j;
	for(i=IMG_ROW-1;i>=0;i--)
	{
		if(i==MODE.foresight) {
			for(j=0;j<=IMG_COL-1;j++)
			{
				if((j==LeftEdge[i])||(j==RightEdge[i])||(j==MidLine[i]))
				UART_WriteByte(DATACOMM_IMG_TRANS_CHL,0x00);
				else
				UART_WriteByte(DATACOMM_IMG_TRANS_CHL,0xfe);
			}
		}
		else {
		for(j=0;j<=IMG_COL-1;j++)
		{
			if((j==LeftEdge[i])||(j==RightEdge[i])||(j==MidLine[i]))
				UART_WriteByte(DATACOMM_IMG_TRANS_CHL,0x00);
			else 
					{
						if(imgBuf[i][j]==0x00)
							UART_WriteByte(HW_UART0,IMG_WHITE);
						else
							UART_WriteByte(HW_UART0,IMG_BLACK);
					}
		}						
	}
}
	UART_WriteByte(DATACOMM_IMG_TRANS_CHL,IMG_FRAME_FIN);
}

