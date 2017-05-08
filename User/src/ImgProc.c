#include "ImgProc.h"
//图像内存池
uint8_t gCCD_RAM[600];   //使用内部RAM
uint8_t imgBuftemp[OV7725_H*OV7725_W];//解压后一维数组
uint8_t imgBuf[OV7725_H][OV7725_W];   //图像二维数组
uint8_t proimgBuf[OV7725_H][OV7725_W];//处理后的二维数组
uint16_t imgProcFlag;
void UserApp(uint32_t vcount);

//压缩二值化图像解压 srclen是二值化图像的占用空间大小
void img_extract(uint8_t *dst, uint8_t *src, uint32_t srclen)
{        
	uint8_t colour[2] = {255, 0};   //colour[0]=255,白色；colour[1]=0,黑色      
	uint8_t tmpsrc;        
	while(srclen --)       
	{                
		tmpsrc = *src++;                
		*dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];                
		*dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];                
		*dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];                
		*dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];               
		*dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];                
		*dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];                
		*dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];                
		*dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];         
	}
}

void UserApp(uint32_t vcount)
{
	  uint16_t i,j;
		uint16_t k = 0;
	imgProcFlag = 0;
	img_extract(imgBuftemp,(uint8_t *) gCCD_RAM , OV7725_W * OV7725_H/8);//图像解压
	//图像校正
//	for(i=0;i<OV7725_H*OV7725_W-9;i++)
//	{
//		imgBuftemp[i] = imgBuftemp[i+9];
//	}
//	vcan_sendimg(imgBuftemp,sizeof (imgBuftemp));
	//转为二维数组
	for(i=0;i<OV7725_H;i++)
	{
		for(j=0;j<OV7725_W;j++)
		{
			imgBuf[i][j] = imgBuftemp[k++];
		}
	}
	//圆环判别
		/*circle identify start*/
  for(i=30;i<OV7725_H;i++)
	{
		if(((imgBuf[i][IMG_MIDPOINT]|imgBuf[i+1][IMG_MIDPOINT]|imgBuf[i+2][IMG_MIDPOINT]|imgBuf[i+3][IMG_MIDPOINT]|imgBuf[i+4][IMG_MIDPOINT]) == 0) && (RightEdge[i-1] - LeftEdge[i-1] > 70) && (RightEdge[i-2] - LeftEdge[i-2] > 70) && (RightEdge[i-3] - LeftEdge[i-3] > 70))
		{
			imgProcFlag = CIRCLE ;
			break;
		}
	}	
	/*circle identify end*/
	
	//搜边线
	for(i=0;i<OV7725_H;i++)
	{
		LeftFlag = LeftBorderSearch(i);
		RightFlag = RightBorderSearch(i);
//		EdgeSearch(i);
		MiddleLineUpdate(i);
	}
	//更新处理后的图像数组，方便发上位机
	for(i=0;i<OV7725_H;i++)
	{
		for(j=0;j<OV7725_W;j++)
		{
			if((j==LeftEdge[i])||(j==RightEdge[i])||(j==MidLine[i]))
				proimgBuf[i][j] = 0;
			else proimgBuf[i][j] = imgBuf[i][j];
		}
	}
	
//	vcan_sendimg((uint8_t *)proimgBuf,sizeof (proimgBuf));
//	vcan_sendimg(gCCD_RAM+1,600);
}

void OV_ISR(uint32_t index)
{
    static uint8_t status = TRANSFER_IN_PROCESS;
    static uint32_t h_counter,v_counter;
		
		  /* ??? */
		if(index & (1 << BOARD_OV7725_VSYNC_PIN))
		{
				GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_RisingEdge, false);
				DMA_SetDestAddress(HW_DMA_CH2,(uint32_t)&gCCD_RAM[0]);
				DMA_SetMajorLoopCounter(HW_DMA_CH2,600);
			//status = TRANSFER_IN_PROCESS;
				switch(status)
				{
						case TRANSFER_IN_PROCESS: 
						{    		UserApp(v_counter++);
										status = NEXT_FRAME;
								break;}
						case NEXT_FRAME: 
						{DMA_EnableRequest(HW_DMA_CH2);
								status =  TRANSFER_IN_PROCESS;
								break;}
						default:
								break;
				}
				PORTC->ISFR = 0xFFFFFFFF;
				h_counter = 0;
				GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_RisingEdge, true);
		}
}

void ImgProcInit(void){
	uint32_t i;
	DMA_InitTypeDef DMA_InitStruct1 = {0};
    
    GPIO_QuickInit(BOARD_OV7725_PCLK_PORT, BOARD_OV7725_PCLK_PIN, kGPIO_Mode_IPD);
		PORT_PinPassiveFilterConfig(BOARD_OV7725_PCLK_PORT,BOARD_OV7725_PCLK_PIN,ENABLE);
		
    GPIO_QuickInit(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_Mode_IPU);
		PORT_PinPassiveFilterConfig(BOARD_OV7725_VSYNC_PORT,BOARD_OV7725_VSYNC_PIN,ENABLE);
		
    GPIO_QuickInit(BOARD_OV7725_HREF_PORT, BOARD_OV7725_HREF_PIN, kGPIO_Mode_IPD);
    
    /* install callback */
    GPIO_CallbackInstall(BOARD_OV7725_VSYNC_PORT, OV_ISR);
   
    //GPIO_ITDMAConfig(BOARD_OV7620_HREF_PORT, BOARD_OV7620_HREF_PIN, kGPIO_IT_FallingEdge, true);
    GPIO_ITDMAConfig(BOARD_OV7725_VSYNC_PORT, BOARD_OV7725_VSYNC_PIN, kGPIO_IT_RisingEdge, true);
    GPIO_ITDMAConfig(BOARD_OV7725_PCLK_PORT, BOARD_OV7725_PCLK_PIN, kGPIO_DMA_FallingEdge, true);
    
    /* 数据口配置*/
    for(i=0;i<8;i++)
    {
        GPIO_QuickInit(HW_GPIOD,i, kGPIO_Mode_IPU);
    }
    
    //DMA配置
    DMA_InitStruct1.chl = HW_DMA_CH2;
    DMA_InitStruct1.chlTriggerSource = PORTC_DMAREQ;
    DMA_InitStruct1.triggerSourceMode = kDMA_TriggerSource_Normal;
    DMA_InitStruct1.minorLoopByteCnt = 1;
    DMA_InitStruct1.majorLoopCnt = 600;
    
    DMA_InitStruct1.sAddr = (uint32_t)&PTD->PDIR;
    DMA_InitStruct1.sLastAddrAdj = 0;
    DMA_InitStruct1.sAddrOffset = 0;
    DMA_InitStruct1.sDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.sMod = kDMA_ModuloDisable;
    
    DMA_InitStruct1.dAddr = (uint32_t)gCCD_RAM;
    DMA_InitStruct1.dLastAddrAdj = 0;
    DMA_InitStruct1.dAddrOffset = 1;
    DMA_InitStruct1.dDataWidth = kDMA_DataWidthBit_8;
    DMA_InitStruct1.dMod = kDMA_ModuloDisable;

    /* initialize DMA moudle */
    DMA_Init(&DMA_InitStruct1);
		DMA_EnableRequest(HW_DMA_CH2);
	}
