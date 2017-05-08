#include "ImgProc.h"
#include "TrackIdentify.h"

uint8_t imgBuf[IMG_ROW][IMG_COL];
uint16_t imgProcFlag;
uint16_t StopFlagAnalyze;
uint16_t StopFlag;

float time;
static uint8_t imgBufRow;
static uint8_t imgRealRow;

static void ImgProcHREF(uint32_t pinxArray);
static void ImgProcVSYN(uint32_t pinxArray);

void ImgProcInit(void) {
    GPIO_QuickInit(CAMERA_HREF_PORT, CAMERA_HREF_PIN, kGPIO_Mode_IPU);
    GPIO_QuickInit(CAMERA_VSYN_PORT, CAMERA_VSYN_PIN, kGPIO_Mode_IPU);
    GPIO_CallbackInstall(CAMERA_HREF_PORT, ImgProcHREF);
    GPIO_CallbackInstall(CAMERA_VSYN_PORT, ImgProcVSYN);
    GPIO_ITDMAConfig(CAMERA_HREF_PORT, CAMERA_HREF_PIN, kGPIO_IT_RisingEdge, DISABLE );
    GPIO_ITDMAConfig(CAMERA_VSYN_PORT, CAMERA_VSYN_PIN, kGPIO_IT_RisingEdge, DISABLE );
    
    GPIO_QuickInit(CAMERA_DATA_PORT, CAMERA_DATA_PIN, kGPIO_Mode_IPU);
	  GPIO_QuickInit(CAMERA_ODEV_PORT, CAMERA_ODEV_PIN, kGPIO_Mode_IPU);
}

uint16_t ImgProcAnalyze(uint8_t imgBuf[IMG_ROW][IMG_COL]) {
	uint16_t ImgProcFlag,i,j;
	static uint16_t ZebraFlag;
	ImgProcFlag = 0 ;
	/*circle identify start*/
  for(i=20;i<40;i++)
	{
		if(((imgBuf[i][IMG_MIDPOINT]|imgBuf[i+1][IMG_MIDPOINT]|imgBuf[i+2][IMG_MIDPOINT]|imgBuf[i+3][IMG_MIDPOINT]|imgBuf[i+4][IMG_MIDPOINT]) ==0) && (RightEdge[i-1] - LeftEdge[i-1] > 220) && (RightEdge[i-2] - LeftEdge[i-2] > 220) && (RightEdge[i-3] - LeftEdge[i-3] > 220))
		{
			ImgProcFlag = CIRCLE ;
			break;
		}
	}
	
	/*circle identify end*/
	
//	if(ImgProcFlag != CIRCLE )
//	{
//		ImgProcFlag = CurveCal(1);
//	}
	
	/*stopline identify start*/
//	if(StopFlagAnalyze)
//	{
//		for(i=0;i<IMG_ROW;i++)
//		{
//			
//			for(j=10;j<IMG_COL-10;j++)
//			{
//				if((imgBuf[i][j] == 0) && (imgBuf[i][j+1] == 1))
//					ZebraFlag++;
//			}
//			if(ZebraFlag > 20)
//			{
//				ZebraFlag = 0;
//				StopFlag++;
//					 break;
//			}
//	  }
//  }
	/*stopline identify end*/
    return ImgProcFlag;
}

void ImgProcHREF(uint32_t pinxArray) {
    int16_t i;
    //if pinxArray & (1 << CAMERA_HREF_PIN) then
    if(imgBufRow < IMG_ROW && imgRealRow > IMG_ABDN_ROW)
    {
        if(!(imgRealRow % IMG_ROW_INTV)) {
            for(i = 0; i <= 90; i++);//140
            for(i = IMG_COL - 1; i >= 0; i--)
                imgBuf[imgBufRow][i] = CAMERA_DATA_READ ;
//            imgBufRow++;
        }
				
				if(imgRealRow % IMG_ROW_INTV ==1) {
					LeftFlag = LeftBorderSearch(imgBufRow);
				}
				
				if(imgRealRow % IMG_ROW_INTV ==2) {
					RightFlag = RightBorderSearch(imgBufRow);
				}
				
				if(imgRealRow % IMG_ROW_INTV ==3) {
					MiddleLineUpdate(imgBufRow);
					imgBufRow++;
				}
				
    }
    imgRealRow++;
}

void ImgProcVSYN(uint32_t pinxArray) {
    //if pinxArray & (1 << CAMERA_VSYN_PIN) then
    imgRealRow = 0;
    imgBufRow = 0;
    imgProcFlag = ImgProcAnalyze(imgBuf);
}
