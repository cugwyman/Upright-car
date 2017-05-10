#include "System.h"
#include "pit.h"

int32_t AC_Out;
int32_t VC_Out;
int32_t DC_Out;
int32_t Left_Out, Right_Out;
#if defined(SINGLE_VC) || defined(VC) || defined(DC)
int32_t speed;
float distance;
#endif

static void NVICInit(void);
static void TimerInit(void);
static void MainProcess(void);
static void BuzzleInit(void);

//#ifdef RS_JUDGE
//static bool startFlag;
//static bool start;
//static bool out;
//#endif

/**
 * @brief  标准总处理, 集中角度环、速度环和方向环, 给予电机标准输出
 */
void MainProcess(void)
{
//		int32_t stopCnt = 0;
//	
		static bool cnt = true;
//	  int16_t VC_Set_Temp;
		time += 0.005;
//	  VC_Set = MODE.VC_Set;
		if(cnt)
		{
				if(time < 1)
						MODE.VC_Set = 0;
				else
				{
						MODE.VC_Set = VC_Set;
						cnt = false;
				}
		}
//		if(time > 0)
//				StopFlagAnalyze = 1;
//    switch ( imgProcFlag ) {
//        case STRAIGHTROAD : { }  break;
//			  case CURVEROAD    : { }  break;
//			}

    #ifndef SINGLE_VC
		dirAngleSpeed = DirGyroGet();
		#ifdef SLOW_DOWN
			if(dirAngleSpeed > TURN_FLAG || dirAngleSpeed < -TURN_FLAG) {
        		if(MODE.VC_Set > VC_Min) {
        			MODE.VC_Set--;
        		} else if(MODE.VC_Set < VC_Min) {
        			MODE.VC_Set++;
        		}
        	} else {
        		if(MODE.VC_Set < VC_Max) {
        			MODE.VC_Set++;
        		} else if(MODE.VC_Set > VC_Max) {
        			MODE.VC_Set--;
				}
			}
		#endif
        #if defined(VC) || defined(DC)
            speed = EncoderGet();
        #endif
        #ifdef AC
            AC_Out = AngleProc();
        #endif
        #ifdef VC
            VC_Out = VelocityProc(speed);
        #endif
        #ifdef DC
            DC_Out = DirectionProc(speed);
        #endif
        #ifdef RS_JUDGE
            if(!start) {
                if(ReedSwitch_Get()) {
                    startFlag = true;
                } else if(!ReedSwitch_Get() && startFlag) {
                    start = true;
                }
            } else {
                if(ReedSwitch_Get()) {
                    out = true;
                }
            }
        #endif
        #if defined(OUT_JUDGE) || defined(RS_JUDGE)
            if(out)
            {
                MOTOR_STOP;
            }
            else
            {
        #endif
//            DC_Out = 0; //直立调试
//						if(DC_Out <= 0) {
//            Left_Out = AC_Out - VC_Out - DC_OUT_COEF_LEFT*DC_Out;
//            Right_Out = AC_Out - VC_Out + DC_OUT_COEF_LEFT*DC_Out;
//                
//            MotorOut(Left_Out, Right_Out+23);
//						}
//						else {
//						Left_Out = AC_Out - VC_Out - DC_OUT_COEF_RIGHT*DC_Out;
//            Right_Out = AC_Out - VC_Out + DC_OUT_COEF_RIGHT*DC_Out;
//                
//            MotorOut(Left_Out, Right_Out+23);
//						}
							/*Stop!!!*/
//							if(StopFlag > 10)
//							{
//								stopCnt++;
//								while(stopCnt > 8000)
//								{
//									MOTOR_STOP;
//								}
//							}
//							else
//							{
								Left_Out = AC_Out - VC_Out + DC_Out;
								Right_Out = AC_Out - VC_Out - DC_Out;

								MotorOut(Left_Out, Right_Out);
//            MotorOut(0, 0);//zuoyoubutong
//							}
        #if defined(OUT_JUDGE) || defined(RS_JUDGE)
            }
        #endif
    #else /* SINGLE_VC */
        speed = EncoderGet();
        VC_Out = VelocityPID(MODE.VC_Set, speed);
        MotorOut(VC_Out, VC_Out);

    #endif /* SINGLE_VC */
}

/*0-3级，抢占最大，0比1高，越小越高*/
void NVICInit() {
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    NVIC_SetPriority(HREF_IRQ, NVIC_EncodePriority(NVIC_PriorityGroup_2, 0, 0));
    NVIC_SetPriority(VSYN_IRQ, NVIC_EncodePriority(NVIC_PriorityGroup_2, 0, 1));
    NVIC_SetPriority(TIMR_IRQ, NVIC_EncodePriority(NVIC_PriorityGroup_2, 1, 0));
}

void TimerInit() {
    PIT_QuickInit(PIT_CHL, PIT_PRD);
    PIT_CallbackInstall(PIT_CHL, MainProcess);
    PIT_ITDMAConfig(PIT_CHL, kPIT_IT_TOF, DISABLE);
}

void BuzzerInit() {
    GPIO_QuickInit(BUZZER_PORT, BUZZER_PIN, kGPIO_Mode_IPU);
}

void GeneralInit(void) {
    DelayInit();
    GearInit();
    UartInit();
    MotorInit();
    CollectInit();  
    EncoderInit();
		BuzzerInit();
//    DisplayInit();
    NVICInit();
    TimerInit();
    ImgProcInit();
    PIT_ITDMAConfig(PIT_CHL, kPIT_IT_TOF, ENABLE);
    GPIO_ITDMAConfig(CAMERA_HREF_PORT, CAMERA_HREF_PIN, kGPIO_IT_RisingEdge, ENABLE );
    GPIO_ITDMAConfig(CAMERA_VSYN_PORT, CAMERA_VSYN_PIN, kGPIO_IT_RisingEdge, ENABLE );
}

