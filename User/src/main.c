/*
 *Up-Right Car
 */
#include "MainProc.h"
#include "DataComm.h"
#include "uart.h"
#include "ImgUtility.h"
#include "gpio.h"

int main(void)
{
		MainInit();
		while(1) {
//			BUZZLE_OFF;
//        ImgTrans(&resultSet);
//		StateTrans(Angle_Kalman , angleSpeedIntegral , angleSpeed , angle);
//		printf("%d   %d     %f      %d\n", state , changeRate, reFuzzy, DC_Out);
        printf("%d\n", resultSet.imgProcFlag);

            
//        MotorOut(3000, 3000);

					DelayMs(50);
//            BUZZLE_ON;
//                    MotorOut(-3000, -3000);

//					DelayMs(100);
//			Oled_PrintfPhoto1(imgBuf);
	}
}

