#include "MainProc.h"
#include "ImgUtility.h"
#include "pit.h"
#include "stdlib.h"
#include "gpio.h"

int32_t AC_Out;
int32_t VC_Out;
int32_t DC_Out;
int32_t Left_Out, Right_Out;
float distance;
float time;

#if defined(SINGLE_VC) || defined(VC) || defined(DC)
int32_t speed;
#endif

static void NVICInit(void);
static void BuzzleInit(void);
static void TimerInit(void);
static void MainProc(void);
//static void SwitchAndParamLoad(void);

void MainInit() 
{
    DelayInit();
//    SwitchAndParamLoad();
    GearInit();    
    MotorInit();   
    CollectInit(); 
    EncoderInit();       
    DataCommInit();
    Oled_Init_n();
//    BuzzleInit();
    NVICInit(); 
    ImgProcInit();     
    TimerInit();
    
    PIT_ITDMAConfig(PIT_CHL, kPIT_IT_TOF, ENABLE);
    GPIO_ITDMAConfig(CAMERA_HREF_PORT, CAMERA_HREF_PIN, kGPIO_IT_RisingEdge, ENABLE );
    GPIO_ITDMAConfig(CAMERA_VSYN_PORT, CAMERA_VSYN_PIN, kGPIO_IT_RisingEdge, ENABLE );
}

void NVICInit() 
{
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    NVIC_SetPriority(HREF_IRQ, NVIC_EncodePriority(NVIC_PriorityGroup_2, 0, 0));
    NVIC_SetPriority(VSYN_IRQ, NVIC_EncodePriority(NVIC_PriorityGroup_2, 0, 1));
    NVIC_SetPriority(TIMR_IRQ, NVIC_EncodePriority(NVIC_PriorityGroup_2, 1, 0));
}

void BuzzleInit() 
{
    GPIO_QuickInit(BUZZLE_PORT, BUZZLE_PIN, kGPIO_Mode_OOD);
    BUZZLE_OFF;
}

void TimerInit() 
{
    PIT_QuickInit(PIT_CHL, PIT_PRD);
    PIT_CallbackInstall(PIT_CHL, MainProc);
    PIT_ITDMAConfig(PIT_CHL, kPIT_IT_TOF, DISABLE);
}

void MainProc()
{
    static bool cnt = true;
    static int16_t ring_time = 0;
    time += 0.005;
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
    dirAngleSpeed = DirGyroGet();
    #ifdef SLOW_DOWN
		    if( ring_time > 0 )
				{
						ring_time--;
						if(MODE.VC_Set > VC_Min) 
						{
								MODE.VC_Set--;
						} 
						else if(MODE.VC_Set < VC_Min) 
						{
								MODE.VC_Set++;
						}
        }
				else
				{
						if( resultSet.imgProcFlag == STRAIGHT_ROAD ) 
						{
								if(MODE.VC_Set < VC_Max) 
								{
										MODE.VC_Set++;
								} 
								else if(MODE.VC_Set > VC_Max) 
								{
										MODE.VC_Set--;
								}
						} 
						else if( resultSet.imgProcFlag == CIRCLE || resultSet.imgProcFlag == RAMP) 
						{
								ring_time = 300;
								if(MODE.VC_Set > VC_Min) 
								{
										MODE.VC_Set--;
								} 
								else if(MODE.VC_Set < VC_Min) 
								{
										MODE.VC_Set++;
								}
						}
						else
						{
								if(MODE.VC_Set > VC_Set) 
								{
										MODE.VC_Set--;
								} 
								else if(MODE.VC_Set < VC_Set) 
								{
										MODE.VC_Set++;
								}
						}
			  }
    #endif

    #if defined(VC) || defined(DC)
        speed = EncoderGet();
	if(inRing || ringEndDelay) 
    {
        ringDistance += speed * 5;
    }
    if(aroundBarrier) 
    {
        barrierDistance += speed * 5;
    }

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

    #if defined(OUT_JUDGE) || defined(RS_JUDGE)
        if(out)
        {
            MOTOR_STOP;
        }
        else
        {
    #endif
            Left_Out = AC_Out - VC_Out + DC_Out;
            Right_Out = AC_Out - VC_Out - DC_Out;

            MotorOut(Left_Out, Right_Out);
//            MotorOut(3000, 3000);
    #if defined(OUT_JUDGE) || defined(RS_JUDGE)
        }
    #endif

//    if(speed_control_on) {
//        SpeedControlProc(leftSpeed, rightSpeed);
//    }
}

//static void SwitchAndParamLoad(void) {
//    motor_on = true;
//    encoder_on = true;
//    speed_control_on = true;
//    direction_control_on = true;
//    steer_actuator_on = true;
//    img_trans_on = false;
//    state_trans_on = true;
//    mode_switch_on = false;
//    
//    speed_control_speed = 111;
//    speed_control_sum_err_max = 2000;
//    
//    speed_control_acc_speed = 90;
//    speed_control_dec_speed = 50;
//    
//    leftPid.targetValue = speed_control_speed;
//    rightPid.targetValue = speed_control_speed;
//    leftPid.kp = 125;
//    leftPid.ki = 5;
//    leftPid.kd = 10;
//    rightPid.kp = 125;
//    rightPid.ki = 5;
//    rightPid.kd = 10;
//    
//    MODE.pre_sight = 28;
//    
//    direction_control_kd = 0.2;
//    direction_control_kpj = 0.025;
//    direction_control_kpc = 0.000133;
//    
//}
