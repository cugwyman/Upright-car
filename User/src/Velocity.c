#include "Velocity.h"
#include "gpio.h"
//#include "TrackIdentify.h"

mode MODE;

int16_t VC_Max;
int16_t VC_Min;
int16_t VC_Set;
int32_t AC_Max;
int32_t AC_Min;
int32_t AC_Set;
int16_t Pre_Sight_Set;

/**
 * @brief  拨码开关获取速度档位, 共4+1档速度, 于头文件Param.h中定义
 */
void GearInit(void)
{
    MODE.AC_Set = 2;//无配重4;12
//	#ifdef SLOW_DOWN
//		MODE.mid_angle = MODE.AC_Set + 5;
//	#endif
//	#ifndef SLOW_DOWN
//		MODE.mid_angle = MODE.AC_Set + 6;
//	#endif
    MODE.VC_Set = 0;
    GPIO_QuickInit(DIP_PORT, DIP1_PIN, kGPIO_Mode_IPU);
    GPIO_QuickInit(DIP_PORT, DIP2_PIN, kGPIO_Mode_IPU);
    GPIO_QuickInit(DIP_PORT, DIP3_PIN, kGPIO_Mode_IPU);
    GPIO_QuickInit(DIP_PORT, DIP4_PIN, kGPIO_Mode_IPU);
    if( !GPIO_ReadBit(DIP_PORT,DIP1_PIN) ) {MODE.ringDir = Right;}
    else                                   {MODE.ringDir = Left;}
    if     ( !GPIO_ReadBit(DIP_PORT,DIP4_PIN) ) { Mode1();}
    else if( !GPIO_ReadBit(DIP_PORT,DIP3_PIN) ) { Mode2();}
    else if( !GPIO_ReadBit(DIP_PORT,DIP2_PIN) ) { Mode3();}
    else                                        { Mode0();}
	VC_Max = MODE.VC_Set + 2;
	VC_Min = MODE.VC_Set - 10;
	VC_Set = MODE.VC_Set;
//    AC_Max = MODE.AC_Set + 6;
//    AC_Min = MODE.AC_Set - 6;
	AC_Set = MODE.AC_Set;
	Pre_Sight_Set = MODE.pre_sight;

}


/*****************before*****************/
void Mode3(void)//不错good?!
{
    MODE.VC_Set = 75;
    MODE.pre_sight = 17;
    
    MODE.DC_PID_P_COEF = 22;
    MODE.DC_P_MIN = 500;
    MODE.DC_P_MAX = 3000;
    MODE.DC_PID_D = 46;
    MODE.DC_Out_MAX = 2500;

    MODE.pre_sight_offset = 3;
    MODE.ring_offset = 68;
    MODE.ring_end_offset = 38;
}

void Mode2(void)//712good
{
    MODE.VC_Set = 75;
    MODE.pre_sight = 17;
    
    MODE.DC_PID_P_COEF = 22;
    MODE.DC_P_MIN = 500;
    MODE.DC_P_MAX = 3000;
    MODE.DC_PID_D = 46;
    MODE.DC_Out_MAX = 2500;

    MODE.pre_sight_offset = 3;
    MODE.ring_offset = 65;
    MODE.ring_end_offset = 35;
}

void Mode1(void)// 新保底
{
    MODE.VC_Set = 75;
    MODE.pre_sight = 16;
    
    MODE.DC_PID_P_COEF = 22;
    MODE.DC_P_MIN = 500;
    MODE.DC_P_MAX = 3000;
    MODE.DC_PID_D = 45;
    MODE.DC_Out_MAX = 2500;

    MODE.pre_sight_offset = 3;
    MODE.ring_offset = 62;
    MODE.ring_end_offset = 32;
}

void Mode0(void)
{
    MODE.VC_Set = 75;
    MODE.pre_sight = 16;
    
    MODE.DC_PID_P_COEF = 22;
    MODE.DC_P_MIN = 500;
    MODE.DC_P_MAX = 3000;
    MODE.DC_PID_D = 45;
    MODE.DC_Out_MAX = 2500;

    MODE.pre_sight_offset = 3;
    MODE.ring_offset = 59;
    MODE.ring_end_offset = 29;
}


/**
 * @brief  速度PID闭环
 * @param[in]  set 设定目标速度, 不可小于0也不应过大, 在头文件Param.h中定义为MODE.VC_Set
 * \param[in]  nextPoint 当前速度值(左右轮平均速度)
 * @retval 速度环输出, 作为标准电机输出的一环
 */
int32_t VelocityPID(int32_t set, int32_t nextpoint) 
{
	float error;
  static float lastError = 0, prevError = 0;
	float P, I, D;
	static int32_t incpid = 0;

	error = set - nextpoint;
//    if(error > 30)
//        error = 30;
//    if(error < -30)
//        error = -30;

	P = VC_PID_P * ( error - lastError );
	I = VC_PID_I * error;
	D = VC_PID_D * ( error - 2 * lastError + prevError );
    if(I > 10)
        I = 0;
    if(I < -10)
        I = 0;

	prevError = lastError;
	lastError = error;
	 
	incpid += P + I + D;
//	incpid = P + I + D;
	 
	return incpid;
}

/**
 * @brief  标准速度环处理, 根据周期平滑输出, 周期在头文件Param.h中定义为VC_PERIOD
 * @param[in]  speed 由编码器采集的速度, 用于PID闭环控制
 * @retval 速度环输出, 作为标准电机输出的一环, 有限幅, 在头文件Param.h中定义为VC_Out_MAX
 */
int32_t VelocityProc(int32_t speed) 
{
    static uint8_t count = 0;
    static int32_t VC_Out_Old = 0, VC_Out_New = 0;
    int32_t VC_Out;
    
    if( count >= VC_PERIOD )//每5次中断才执行一次，其余时候平滑输出
    {
        count = 0;
        VC_Out_Old = VC_Out_New;
        VC_Out_New = VelocityPID(MODE.VC_Set, speed);
//        VC_Out_New = VelocityPID(30, speed);        
        if( VC_Out_New > VC_Out_MAX )
            VC_Out_New = VC_Out_MAX;
        else if( VC_Out_New < -VC_Out_MAX )
            VC_Out_New = -VC_Out_MAX;
    }
    count++;
    
    VC_Out = VC_Out_Old + (VC_Out_New - VC_Out_Old) * count / VC_PERIOD;
    return VC_Out;
}
