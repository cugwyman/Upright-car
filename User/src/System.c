#include "System.h"
#include "pit.h"
#include "gpio.h"

int32_t AC_Out;
int32_t VC_Out;
int32_t DC_Out;
int32_t Left_Out, Right_Out;
#if defined(SINGLE_VC) || defined(VC) || defined(DC)
int32_t speed;
float distance;
#endif

static void TimerInit(void);
static void MainProcess(void);

#ifdef RS_JUDGE
static bool startFlag;
static bool start;
static bool out;
#endif

/**
 * @brief  标准总处理, 集中角度环、速度环和方向环, 给予电机标准输出
 */
void MainProcess(void)
{
    #ifndef SINGLE_VC
		dirAngleSpeed = DirGyro_Get();
		#ifdef SLOW_DOWN
			if(dirAngleSpeed > TURN_FLAG || dirAngleSpeed < -TURN_FLAG) {
				if(VC_Set > VC_Min) {
					VC_Set--;
				} else if(VC_Set < VC_Min) {
					VC_Set++;
				}
			} else {
				if(VC_Set < VC_Max) {
					VC_Set++;
				} else if(VC_Set > VC_Max) {
					VC_Set--;
				}
			}
		#endif
        #if defined(VC) || defined(DC)
            speed = EncoderGet();
        #endif
        #ifdef AC
            AC_Out = Angle_Process();
        #endif
        #ifdef VC
            VC_Out = Velocity_Process(speed);
        #endif
        #ifdef DC
            DC_Out = Direction_Process(speed);
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
            Left_Out = AC_Out - VC_Out - DC_Out;
            Right_Out = AC_Out - VC_Out + DC_Out;
            MotorOut(Left_Out, Right_Out);
        #if defined(OUT_JUDGE) || defined(RS_JUDGE)
            }
        #endif
    #else /* SINGLE_VC */
        speed = EncoderGet();
        VC_Out = Velocity_PID(VC_Set, speed);
        MotorOut(VC_Out, VC_Out);
    #endif /* SINGLE_VC */
}


static void TimerInit(void)
{
    PIT_QuickInit(HW_PIT_CH0, 5000);//5ms中断
    PIT_CallbackInstall(HW_PIT_CH0, MainProcess);
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, ENABLE);
}

void GeneralInit(void)
{
    DelayInit();
    UartInit();
    MotorInit();
    EncoderInit();
//	FtmQD_Init();
    TimerInit();
}

