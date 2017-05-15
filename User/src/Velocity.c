#include "Velocity.h"
#include "gpio.h"
//#include "TrackIdentify.h"

mode MODE;

int16_t VC_Max;
int16_t VC_Min;
int16_t VC_Set;

/**
 * @brief  拨码开关获取速度档位, 共4+1档速度, 于头文件Param.h中定义
 */
void GearInit(void)
{
    MODE.VC_Set = 0;
    GPIO_QuickInit(DIP_PORT, DIP1_PIN, kGPIO_Mode_IPU);
    GPIO_QuickInit(DIP_PORT, DIP2_PIN, kGPIO_Mode_IPU);
    GPIO_QuickInit(DIP_PORT, DIP3_PIN, kGPIO_Mode_IPU);
    GPIO_QuickInit(DIP_PORT, DIP4_PIN, kGPIO_Mode_IPU);
    if     ( !GPIO_ReadBit(DIP_PORT,DIP4_PIN) ) { Mode1();}
    else if( !GPIO_ReadBit(DIP_PORT,DIP3_PIN) ) { Mode2();}
    else if( !GPIO_ReadBit(DIP_PORT,DIP2_PIN) ) { Mode3();}
    else if( !GPIO_ReadBit(DIP_PORT,DIP1_PIN) ) { Mode4();}
    else                                        { Mode0();}
		VC_Max = MODE.VC_Set;
		VC_Min = MODE.VC_Set * 3 / 4;
		VC_Set = MODE.VC_Set;
}

void Mode4(void)//不错good?!
{
    MODE.VC_Set = 66;
    MODE.pre_sight = 20;
    
    MODE.DC_PID_P_COEF = 61;
    MODE.DC_P_MIN = 1000;
    MODE.DC_P_MAX = 6000;
    MODE.DC_PID_D = 4;
}

void Mode3(void)//不错good?!
{
    MODE.VC_Set = 66;
    MODE.pre_sight = 20;
    
    MODE.DC_PID_P_COEF = 59;
    MODE.DC_P_MIN = 1000;
    MODE.DC_P_MAX = 6000;
    MODE.DC_PID_D = 4;
}
    
void Mode2(void)//不错good?!
{
    MODE.VC_Set = 66;
    MODE.pre_sight = 20;
    
    MODE.DC_PID_P_COEF = 57;
    MODE.DC_P_MIN = 1000;
    MODE.DC_P_MAX = 4800;
    MODE.DC_PID_D = 4;
}

void Mode1(void)
{
    MODE.VC_Set = 66;
    MODE.pre_sight = 20;
    
    MODE.DC_PID_P_COEF = 55;
    MODE.DC_P_MIN = 1000;
    MODE.DC_P_MAX = 4800;
    MODE.DC_PID_D = 4;
}

void Mode0(void)
{
    MODE.VC_Set = 20;
    MODE.pre_sight = 6;
    
    MODE.DC_PID_P_COEF = 50;
    MODE.DC_P_MIN = 1000;
    MODE.DC_P_MAX = 4800;
    MODE.DC_PID_D = 4;
}

/**
 * @brief  速度PID闭环
 * @param[in]  set 设定目标速度, 不可小于0也不应过大, 在头文件Param.h中定义为MODE.VC_Set
 * \param[in]  nextPoint 当前速度值(左右轮平均速度)
 * @retval 速度环输出, 作为标准电机输出的一环
 */
int32_t VelocityPID(int32_t set, int32_t nextpoint) {
	float error;
  static float lastError = 0, prevError = 0;
	float P, I, D;
	static int32_t incpid = 0;

	error = set - nextpoint;
	P = VC_PID_P * ( error - lastError );
	I = VC_PID_I * error;
	D = VC_PID_D * ( error - 2 * lastError + prevError );
	
	prevError = lastError;
	lastError = error;
	 
	incpid += P + I + D;
	 
	return incpid;
}

/**
 * @brief  标准速度环处理, 根据周期平滑输出, 周期在头文件Param.h中定义为VC_PERIOD
 * @param[in]  speed 由编码器采集的速度, 用于PID闭环控制
 * @retval 速度环输出, 作为标准电机输出的一环, 有限幅, 在头文件Param.h中定义为VC_Out_MAX
 */
int32_t VelocityProc(int32_t speed) {
    static uint8_t count = 0;
    static int32_t VC_Out_Old = 0, VC_Out_New = 0;
    int32_t VC_Out;
    
    if( count >= VC_PERIOD )//每5次中断才执行一次，其余时候平滑输出
    {
        count = 0;
        VC_Out_Old = VC_Out_New;
        VC_Out_New = VelocityPID(MODE.VC_Set, speed);
    }
    count++;
    
    VC_Out = VC_Out_Old + (VC_Out_New - VC_Out_Old) * count / VC_PERIOD;
    return VC_Out;
}
