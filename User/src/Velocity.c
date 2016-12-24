#include "Velocity.h"
#include "gpio.h"

int VC_Max;
int VC_Min;
int VC_Set;

//#if VC_Set < 0
//#error "VC_Set is invalid!"
//#endif

/**
 * @brief  拨码开关获取速度档位, 共4+1档速度, 于头文件Param.h中定义
 */
void Gear_Get(void)
{
    VC_Set = 0;
    GPIO_QuickInit(DIP_PORT, DIP1_PIN, kGPIO_Mode_IPU);
    GPIO_QuickInit(DIP_PORT, DIP2_PIN, kGPIO_Mode_IPU);
    GPIO_QuickInit(DIP_PORT, DIP3_PIN, kGPIO_Mode_IPU);
    GPIO_QuickInit(DIP_PORT, DIP4_PIN, kGPIO_Mode_IPU);
    if     ( !GPIO_ReadBit(DIP_PORT,DIP4_PIN) ) { VC_Set = VSET_GEAR4; }
    else if( !GPIO_ReadBit(DIP_PORT,DIP3_PIN) ) { VC_Set = VSET_GEAR3; }
    else if( !GPIO_ReadBit(DIP_PORT,DIP2_PIN) ) { VC_Set = VSET_GEAR2; }
    else if( !GPIO_ReadBit(DIP_PORT,DIP1_PIN) ) { VC_Set = VSET_GEAR1; }
    else                                        { VC_Set = VSET_GEAR0; }
	VC_Max = VC_Set;
	VC_Min = VC_Set * 3 / 4;
}

/**
 * @brief  速度PID闭环
 * @param[in]  set 设定目标速度, 不可小于0也不应过大, 在头文件Param.h中定义为VC_Set
 * \param[in]  nextPoint 当前速度值(左右轮平均速度)
 * @retval 速度环输出, 作为标准电机输出的一环
 */
int32_t Velocity_PID(int32_t set, int32_t nextpoint)
{
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
int32_t Velocity_Process(int32_t speed)
{
    static uint8_t count = 0;
    static int32_t VC_Out_Old = 0, VC_Out_New = 0;
    int32_t VC_Out;
    
    if( count >= VC_PERIOD )
    {
        count = 0;
        VC_Out_Old = VC_Out_New;
        VC_Out_New = Velocity_PID(VC_Set, speed);
        if( VC_Out_New > VC_Out_MAX )
        {
            VC_Out_New = VC_Out_MAX;
        }
        else if( VC_Out_New < -VC_Out_MAX )
        {
            VC_Out_New = -VC_Out_MAX;
        }
    }
    count++;
    
    VC_Out = VC_Out_Old + (VC_Out_New - VC_Out_Old) * count / VC_PERIOD;
    return VC_Out;
}
